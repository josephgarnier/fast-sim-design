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

#ifndef FAST_SIM_DESIGN_MONITORABLE_H
#define FAST_SIM_DESIGN_MONITORABLE_H

#include "frame.h"

namespace FastSimDesign {
namespace SimMonitor {
class Monitor;
class Window;
class Monitorable
{
public:
  explicit Monitorable() = default;
  Monitorable(Monitorable const&) = default;
  Monitorable(Monitorable&&) = default;
  Monitorable& operator=(Monitorable const&) = default;
  Monitorable& operator=(Monitorable&&) = default;
  virtual ~Monitorable() = default;

  virtual void monitorState(Monitor& monitor, Frame& frame_object) const;
  virtual void monitorState(
      Monitor& monitor, Frame::StateMachine& frame_object) const;
  virtual void monitorState(
      Monitor& monitor, Frame::StateMachineState& frame_object) const;
  virtual void monitorState(Monitor& monitor, Frame::World& frame_object) const;
  virtual void monitorState(
      Monitor& monitor, Frame::SceneNode& frame_object) const;
};
} // namespace SimMonitor
} // namespace FastSimDesign
#endif