////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "game.h"

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Statics
  ////////////////////////////////////////////////////////////
  float const Game::PLAYER_SPEED = 100.f;
  sf::Time const Game::TIME_PER_FRAME = sf::seconds(1.f / 60.f);

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  Game::Game()
    : m_window{sf::VideoMode{800, 600}, "FastSimDesign"}
    , m_world{m_window}
    , m_player{}
    , m_font{}
    , m_statistics_text{}
    , m_statistics_update_time{}
    , m_statistics_num_frames{0}
  {
  }

  void Game::run() noexcept
  {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_window.isOpen())
    {
      processEvents();
      timeSinceLastUpdate += clock.restart();
      while (timeSinceLastUpdate > Game::TIME_PER_FRAME)
      {
        timeSinceLastUpdate -= Game::TIME_PER_FRAME;
        processEvents();
        update(Game::TIME_PER_FRAME);
      }
      updateStatistic(timeSinceLastUpdate);
      render();
    }
  }

  void Game::processEvents() noexcept
  {
    CommandQueue& commands = m_world.getCommandQueue();

    sf::Event event;
    while (m_window.pollEvent(event))
    {
      m_player.handleEvent(event, commands);
      if (event.type == sf::Event::Closed)
        m_window.close();
    }
    m_player.handleRealtimeInput(commands);
  }

  void Game::update(sf::Time const& elapsed_time) noexcept
  {
    m_world.update(elapsed_time);
  }

  void Game::render() noexcept
  {
    m_window.clear();
    m_world.draw();

    m_window.setView(m_window.getDefaultView());
    m_window.draw(m_statistics_text);
    m_window.display();
  }

  void Game::updateStatistic(sf::Time const& elapsed_time)
  {
    m_statistics_update_time += elapsed_time;
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
}