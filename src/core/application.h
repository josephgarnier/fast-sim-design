////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#pragma once

#ifndef FAST_SIM_DESIGN_APPLICATION_H
#define FAST_SIM_DESIGN_APPLICATION_H

#include "../entity/player.h"
#include "../state_machine/state_stack.h"
#include "../imgui_gui/imgui_layer.h"
#include "resource_identifiers.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// @brief Application class from which Fast-Sim-Design simulations starts.
  ///
  /// @details
  /// The entry point for Fast-Sim-Design simulations is the Application class and {@link #launch(Application)} method.
  /// It initializes application instance and starts the main application loop.
  ///
  /// @section Life-cycle
  /// <ol>
  ///     <li>{@link #configure(Configuration)} It's called before simulation initialization, so only basic application setups are expected.</li>
  ///     <li>{@link #initWindow(Configuration)} Method creates and initialize SFML window.</li>
  ///     <li>{@link #initImGui(Configuration)} Method initializes Dear ImGui context. Could be used to do Dear ImGui setup as well.</li>
  ///     <li>{@link #preRun()} Method called once, before simulation loop.</li>
  ///     <li>{@link #beginFrame()} Method called at the beginning of every frame, before {@link #processInput()}.</li>
  ///     <li>{@link #processInput()} Method called every frame, before {@link #process()}, to process user inputs.</li>
  ///     <li>{@link #update()} Method called every frame to update simulation state and Dear ImGui states from input.</li>
  ///     <li>{@link #updateStatistic()} Method called every frame to update basic simulation statistic.</li>
  ///     <li>{@link #render()} Method called every frame to render simulation state and Dear ImGui states.</li>
  ///     <li>{@link #endFrame()} Method called at the end of every frame, after {@link #render()}.</li>
  ///     <li>{@link #postRun()} Method called once, after simulation loop.</li>
  ///     <li>{@link #disposeImGui()} Destroys Dear ImGui context.</li>
  ///     <li>{@link #disposeWindow()} Destroys SFML window.</li>
  /// </ol>
  ///
  /// @section Reference
  /// This class in inspired from \link https://github.com/SpaiR/imgui-java/blob/main/imgui-app/src/main/java/imgui/app/Application.java \endlink and \link https://github.com/SpaiR/imgui-java/blob/main/imgui-app/src/main/java/imgui/app/Window.java \endlink.
  ////////////////////////////////////////////////////////////
  struct Configuration;
  class Application
  {
  public:
    /// Entry point of application. Use it to start the simulation.
    /// @param app The instance of the Application class to launch.
    static void launch(Application app);
    static sf::Time const TIME_PER_FRAME;

  private:
  public:
    explicit Application(); // Default constructor, throw CoreException
    virtual ~Application() = default; // Destructor

    ImGuiLayer& getImGuiLayer() noexcept { return m_imgui_layer; }
    StateStack const& getStateMachine() const noexcept { return m_state_stack; }

  protected:
    /// Method called before simulation initialization. Could be used to provide basic window information in editing `config`, like title name, main window size, etc.
    /// @param config The configuration object with basic application information.
    void configure(Configuration& config) const;

    /// Method to initialize application.
    /// @param config The configuration object with basic application information.
    void init(Configuration const& config);

    /// Method to create and initialize SFML window.
    /// @param config The configuration object with basic application information.
    void initWindow(Configuration const& config) noexcept;

    /// Method called once, before simulation loop.
    void preRun() noexcept;

    /// Main simulation loop.
    void run();

    /// Method used to run the next simulation frame.
    /// @param loop_clock The clock object used to measure time between frames.
    /// @param time_since_last_update The time elapsed since last frame.
    void runFrame(sf::Clock& loop_clock, sf::Time& time_since_last_update);

    /// Method called at the beginning of every frame.
    void beginFrame() noexcept;

    /// Method called every frame to process user inputs.
    void processInput();

    /// Method called every frame to update simulation state and Dear ImGui states from input.
    /// @param dt The time elapsed since last frame.
    void update(sf::Time const& dt);

    /// Method called every frame to update basic simulation statistic.
    /// @param dt The time elapsed since last frame.
    void updateStatistic(sf::Time const& dt) noexcept;

    /// Method called every frame to render simulation state and Dear ImGui states.
    void render();

    ///  Method called at the end of every frame.
    void endFrame() noexcept;

    /// Method called once, after simulation loop.
    void postRun() noexcept;

    /// Method to dispose all used application resources and destroy its window.
    void dispose();

    /// Method to destroy SFML window.
    void disposeWindow() noexcept;

  private:
    void registerStates();

  private:
    sf::RenderWindow m_window;
    TextureHolder m_textures;
    FontHolder m_fonts;
    Player m_player;

    StateStack m_state_stack;

    sf::Text m_statistics_text;
    sf::Time m_statistics_update_time;
    std::size_t m_statistics_num_frames;
    std::size_t m_statistics_total_frames;
    std::size_t m_statistics_total_update_frames;
    std::size_t m_statistics_total_render_frames;

    ImGuiLayer m_imgui_layer;
  };
}
#endif
