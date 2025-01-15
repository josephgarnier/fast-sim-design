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

#ifndef FAST_SIM_DESIGN_IMGUI_LAYER_H
#define FAST_SIM_DESIGN_IMGUI_LAYER_H

#include "../core/configuration.h"
#include "modal/modal_dialog.h"
#include "window/controller_window.h"
#include "window/log_window.h"
#include "window/state_machine_window.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

namespace FastSimDesign {
  class ImGuiLayer
  {
  public:
    explicit ImGuiLayer() noexcept; // Default constructor
    ImGuiLayer(ImGuiLayer const&) = default; // Copy constructor
    ImGuiLayer(ImGuiLayer&&) = default; // Move constructor
    ImGuiLayer& operator=(ImGuiLayer const&) = default; // Copy assignment operator
    ImGuiLayer& operator=(ImGuiLayer&&) = default; // Move assignment operator
    virtual ~ImGuiLayer() = default; // Destructor

    ControllerWindow& getControllerWindow() noexcept { return m_controller_window; }
    LogWindow& getLogWindow() noexcept { return m_log_window; }
    StateMachineWindow& getStateMachineWindow() noexcept { return m_state_machine_window; }

    /// Method to initialize Dear ImGui context. Could be overridden to do custom Dear ImGui setup before application start.
    /// @param config The configuration object with basic application information.
    /// @param window The SFML window.
    virtual void initImGui(Configuration const& config, sf::RenderWindow& window) const;

    void handleEvent(sf::RenderWindow& window, sf::Event const& event) const;
    void startImGuiUpdate(sf::RenderWindow& window, sf::Time const& dt);
    void updateImGui(sf::RenderWindow& window, sf::Time const& dt);
    void endImGuiUpdate();
    void renderImGui(sf::RenderWindow& window);

    /// Method to destroy Dear ImGui context.
    virtual void disposeImGui();

    void openModalDialog(std::unique_ptr<ModalDialog>&& modal_dialog);

  protected:
  private:
    void setSpectrumStyle() const;
    void createDockSpace() const;
    ImGuiID configureDockSpace() const;
    void configureDockNodes(ImGuiID dockspace_id) const;

  public:
  protected:
  private:
    ControllerWindow m_controller_window;
    LogWindow m_log_window;
    StateMachineWindow m_state_machine_window;

    std::vector<std::unique_ptr<ModalDialog>> m_open_modal_dialogs;
  };
}
#endif