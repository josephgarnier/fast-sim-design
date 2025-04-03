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

#ifndef FAST_SIM_DESIGN_FRAME_H
#define FAST_SIM_DESIGN_FRAME_H

#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace FastSimDesign {
namespace SimMonitor {
struct Frame
{
  struct StateMachineState
  {
    std::string_view name;
  };

  struct StateMachine
  {
    std::vector<StateMachineState> states;
  };

  struct SceneNode
  {
    using Ptr = std::unique_ptr<SceneNode>;

    std::string category = "UNKNOWN";
    std::string type = "NO TYPE";
    SceneNode* m_parent = nullptr;
    std::vector<Ptr> m_children;

    void attachChild(Ptr child);
    Ptr detachChild(SceneNode const& child);
  };

  struct World
  {
  };

  StateMachine state_stack;
  World world;
  SceneNode scene_graph;
};
} // namespace SimMonitor
} // namespace FastSimDesign
#endif