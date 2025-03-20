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

#include "window.h"
#include "../frame.h"

namespace FastSimDesign {
  namespace SimMonitor {
    class SceneGraphWindow final : public Window
    {
    private:
      using Parent = Window;

    public:
      explicit SceneGraphWindow(Monitor* monitor); // Default constructor
      SceneGraphWindow(SceneGraphWindow const&) = default; // Copy constructor
      SceneGraphWindow(SceneGraphWindow&&) = default; // Move constructor
      SceneGraphWindow& operator=(SceneGraphWindow const&) = default; // Copy assignment operator
      SceneGraphWindow& operator=(SceneGraphWindow&&) = default; // Move assignment operator
      virtual ~SceneGraphWindow() = default; // Destructor

    private:
      virtual void updateMenuBar(sf::Time const& dt) override;
      virtual void updateContentArea(sf::Time const& dt) override;
      
      void drawTreeSceneNode(Frame::SceneNode const& frame_scene_node, uintmax_t& id) const;
    };
  }
}
#endif