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

#ifndef FAST_SIM_DESIGN_STATE_MACHINE_WINDOW_H
#define FAST_SIM_DESIGN_STATE_MACHINE_WINDOW_H

#include "window.h"

namespace FastSimDesign {
  class StateStack;
  class StateMachineWindow : public Window
  {
  private:
    using Parent = Window;

  public:
    explicit StateMachineWindow(Application* app) noexcept; // Default constructor
    StateMachineWindow(StateMachineWindow const&) = default; // Copy constructor
    StateMachineWindow(StateMachineWindow&&) = default; // Move constructor
    StateMachineWindow& operator=(StateMachineWindow const&) = default; // Copy assignment operator
    StateMachineWindow& operator=(StateMachineWindow&&) = default; // Move assignment operator
    virtual ~StateMachineWindow() = default; // Destructor
  protected:
  private:
    virtual void draw(sf::RenderWindow& window, sf::Time const& dt) override;
    
  private:
    StateStack const * m_state_machine;
  };
}
#endif