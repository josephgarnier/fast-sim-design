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
  namespace SimMonitor {
    class ControllerWindow final : public Window
    {
    private:
      using Parent = Window;

    public:
      explicit ControllerWindow(Monitor* monitor) noexcept; // Default constructor
      ControllerWindow(ControllerWindow const&) = default; // Copy constructor
      ControllerWindow(ControllerWindow&&) = default; // Move constructor
      ControllerWindow& operator=(ControllerWindow const&) = default; // Copy assignment operator
      ControllerWindow& operator=(ControllerWindow&&) = default; // Move assignment operator
      virtual ~ControllerWindow() = default; // Destructor
    private:
      virtual void updateMenuBar(sf::Time const& dt) override;
      virtual void updateContentArea(sf::Time const& dt) override;

    private:
      bool m_show_entity_inspector{true};
      bool m_show_debug_window{true};
      bool m_show_imgui_demo{false};
    };
  }
}
#endif