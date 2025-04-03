////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "monitorable.h"

#include "window/window.h"

namespace FastSimDesign {
namespace SimMonitor {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
void Monitorable::monitorState(Monitor&, Frame&) const
{
  // Do nothing by default.
}

void Monitorable::monitorState(Monitor&, Frame::StateMachine&) const
{
  // Do nothing by default.
}

void Monitorable::monitorState(Monitor&, Frame::StateMachineState&) const
{
  // Do nothing by default.
}

void Monitorable::monitorState(Monitor&, Frame::World&) const
{
  // Do nothing by default.
}

void Monitorable::monitorState(Monitor&, Frame::SceneNode&) const
{
  // Do nothing by default.
}

} // namespace SimMonitor
} // namespace FastSimDesign