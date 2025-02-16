////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "application.h"
#include "../state_machine/state_identifiers.h"
#include "../state_machine/title_state.h"
#include "../state_machine/menu_state.h"
#include "../state_machine/game_state.h"
#include "../state_machine/pause_state.h"
#include "configuration.h"
#include "log.h"

#include <SFML/System/Sleep.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/WindowStyle.hpp>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Statics
  ////////////////////////////////////////////////////////////
  sf::Time const Application::TIME_PER_FRAME = sf::seconds(1.f / 60.f);

  void Application::launch(Application app)
  {
    LOG_INFO("Starting FastSimDesign...");
    Configuration config{};
    app.configure(config);
    app.init(config);
    app.preRun();
    LOG_INFO("FastSimDesign started successfully!");
    app.run();
    LOG_INFO("Stopping FastSimDesign...");
    app.postRun();
    app.dispose();
    LOG_INFO("FastSimDesign stopped successfully!");
  }

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  Application::Application()
    : m_window{sf::VideoMode{1280, 720}, "Fast-Sim-Design", sf::Style::Default}
    , m_textures{}
    , m_fonts{}
    , m_player{}
    , m_state_stack{State::Context{&m_window, &m_textures, &m_fonts, &m_player}}
    , m_statistics_text{}
    , m_statistics_update_time{}
    , m_statistics_num_frames{0}
    , m_imgui_layer{this}
  {
    m_window.setKeyRepeatEnabled(false);
    m_window.setFramerateLimit(60);

    m_fonts.load(Fonts::ID::MAIN, "../assets/fonts/arial.ttf");
    m_textures.load(Textures::ID::TITLE_SCREEN, "../assets/sprites/npcs/title_screen.png");

    m_statistics_text.setFont(m_fonts.get(Fonts::ID::MAIN));
    m_statistics_text.setPosition(5.f, 5.f);
    m_statistics_text.setCharacterSize(10);

    registerStates();
    m_state_stack.pushState(States::ID::TITLE);
  }

  void Application::configure(Configuration&) const
  {
    LOG_INFO("*** (1/3) Configurating app ***");
    LOG_INFO("App configurated.");
  }

  void Application::init(Configuration const& config)
  {
    LOG_INFO("*** (2/3) Initializing app ***");
    initWindow(config);
    m_imgui_layer.initImGui(config, m_window);
    LOG_INFO("App initialized.");
  }

  void Application::initWindow(Configuration const& config) noexcept
  {
    sf::Uint32 style = sf::Style::Default;
    if (config.fullscreen)
      style = sf::Style::Fullscreen;
    m_window.create(sf::VideoMode{config.width, config.height}, config.title, style);
  }

  void Application::preRun() noexcept
  {
    LOG_INFO("*** (3/3) Pre-running app ***");
    LOG_INFO("App pre-ran.");
  }

  void Application::run()
  {
    LOG_INFO("*** Run app with a tick timer at {}ms per frame ***", Application::TIME_PER_FRAME.asMilliseconds());

    sf::Clock clock;
    sf::Time time_since_last_update = sf::Time::Zero;
    while (m_window.isOpen())
    {
      runFrame(clock, time_since_last_update);
    }
  }

  void Application::runFrame(sf::Clock& loop_clock, sf::Time& time_since_last_update)
  {
    LOG_TRACE("*** Previous frame duration is {}ms ***", loop_clock.getElapsedTime().asMilliseconds());
    sf::Time elapsed_time = loop_clock.restart();
    beginFrame();
    time_since_last_update += elapsed_time;
    if (time_since_last_update > Application::TIME_PER_FRAME)
    {
      do
      {
        time_since_last_update -= Application::TIME_PER_FRAME;
        processInput();
        update(Application::TIME_PER_FRAME);

        if (m_state_stack.isEmpty())
          m_window.close();
      } while (time_since_last_update > Application::TIME_PER_FRAME);
      updateStatistic(elapsed_time);
      render();
    } else
    {
      sf::sleep(Application::TIME_PER_FRAME - time_since_last_update - loop_clock.getElapsedTime());
    }
    endFrame();
  }

  void Application::beginFrame() noexcept
  {
    LOG_TRACE("***** Begin frame {} *****", m_statistics_num_frames);
  }

  void Application::processInput()
  {
    LOG_TRACE("Process inputs.");
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      m_imgui_layer.handleEvent(m_window, event);
      m_state_stack.handleEvent(event);
      if (event.type == sf::Event::Closed)
        m_window.close();
    }
  }

  void Application::update(sf::Time const& dt)
  {
    LOG_TRACE("Update app wih a delta time of {}ms.", dt.asMilliseconds());

    // Update stack.
    m_state_stack.update(dt);

    // Update ImGui.
    m_imgui_layer.startImGuiUpdate(m_window, dt);
    m_imgui_layer.updateImGui(m_window, dt);
    // m_state_stack.updateImGui(dt); TODO to implement layer : each layer can have its own ImGui Window.
    m_imgui_layer.endImGuiUpdate();
  }

  void Application::updateStatistic(sf::Time const& dt) noexcept
  {
    m_statistics_update_time += dt;
    m_statistics_num_frames += 1;

    if (m_statistics_update_time >= sf::seconds(1.0f))
    {
      m_statistics_text.setString(
        "Frames / Second = " + std::to_string(m_statistics_num_frames) + "\n" +
        "Time / Update = " + std::to_string(static_cast<size_t>(m_statistics_update_time.asMicroseconds()) / m_statistics_num_frames) + "us");

      m_statistics_update_time -= sf::seconds(1.0f);
      m_statistics_num_frames = 0;
    }
  }

  void Application::render()
  {
    m_window.clear();
    m_state_stack.draw();

    m_window.setView(m_window.getDefaultView());
    m_window.draw(m_statistics_text);
    m_imgui_layer.renderImGui(m_window);
    m_window.display();
  }

  void Application::endFrame() noexcept
  {
    LOG_TRACE("***** End frame {} *****", m_statistics_num_frames);
  }

  void Application::postRun() noexcept
  {
    LOG_INFO("*** (1/2) Post-running app ***");
    LOG_INFO("App post-ran.");
  }

  void Application::dispose()
  {
    LOG_INFO("*** (2/2) Dispose app ***");
    m_imgui_layer.disposeImGui();
    disposeWindow();
    LOG_INFO("App disposed.");
  }

  void Application::disposeWindow() noexcept
  {
  }

  void Application::registerStates()
  {
    m_state_stack.registerState<TitleState>(States::ID::TITLE);
    m_state_stack.registerState<MenuState>(States::ID::MENU);
    m_state_stack.registerState<GameState>(States::ID::GAME);
    m_state_stack.registerState<PauseState>(States::ID::PAUSE);
  }
}