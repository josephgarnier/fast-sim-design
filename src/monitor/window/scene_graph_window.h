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

#ifndef FAST_SIM_DESIGN_SCENE_GRAPH_WINDOW_H
#define FAST_SIM_DESIGN_SCENE_GRAPH_WINDOW_H

#include "../frame.h"
#include "window.h"

namespace FastSimDesign {
namespace SimMonitor {
class SceneGraphWindow final : public Window
{
private:
  using Parent = Window;

public:
  explicit SceneGraphWindow(Monitor* monitor);
  SceneGraphWindow(SceneGraphWindow const&) = default;
  SceneGraphWindow(SceneGraphWindow&&) = default;
  SceneGraphWindow& operator=(SceneGraphWindow const&) = default;
  SceneGraphWindow& operator=(SceneGraphWindow&&) = default;
  virtual ~SceneGraphWindow() = default;

private:
  virtual void updateMenuBar(sf::Time const& dt) override;
  virtual void updateContentArea(sf::Time const& dt) override;

  void drawTreeSceneNode(
      Frame::SceneNode const& frame_scene_node, uintmax_t& id) const;
};
} // namespace SimMonitor
} // namespace FastSimDesign
#endif