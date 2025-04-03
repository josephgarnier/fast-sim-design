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
namespace SimMonitor {
class StateStack;
class StateMachineWindow final : public Window
{
private:
  using Parent = Window;

public:
  explicit StateMachineWindow(Monitor* monitor) noexcept;
  StateMachineWindow(StateMachineWindow const&) = default;
  StateMachineWindow(StateMachineWindow&&) = default;
  StateMachineWindow& operator=(StateMachineWindow const&) = default;
  StateMachineWindow& operator=(StateMachineWindow&&) = default;
  virtual ~StateMachineWindow() = default;

private:
  virtual void updateMenuBar(sf::Time const& dt) override;
  virtual void updateContentArea(sf::Time const& dt) override;
};
} // namespace SimMonitor
} // namespace FastSimDesign
#endif