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

#ifndef FAST_SIM_DESIGN_CONTROLLER_WINDOW_H
#define FAST_SIM_DESIGN_CONTROLLER_WINDOW_H

#include "window.h"

namespace FastSimDesign {
  class ControllerWindow : public Window
  {
    private:
      using Parent = Window;
      
  public:
    explicit ControllerWindow(ImGuiLayer* imgui_layer) noexcept; // Default constructor, throw GuiException
    ControllerWindow(ControllerWindow const&) = default; // Copy constructor
    ControllerWindow(ControllerWindow&&) = default; // Move constructor
    ControllerWindow& operator=(ControllerWindow const&) = default; // Copy assignment operator
    ControllerWindow& operator=(ControllerWindow&&) = default; // Move assignment operator
    virtual ~ControllerWindow() = default; // Destructor
  private:
    virtual void draw(sf::RenderWindow& window, sf::Time const& dt) noexcept override;

  protected:
  private:
    bool m_show_entity_inspector;
    bool m_show_debug_window;
    bool m_show_state_machine_window;
    bool m_show_imgui_demo;
  };
}
#endif