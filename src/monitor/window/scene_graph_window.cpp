////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "scene_graph_window.h"
#include "../monitorable.h"
#include "../../utils/imgui_util.h"

#include <imgui.h>

namespace FastSimDesign {
  namespace SimMonitor {
    ////////////////////////////////////////////////////////////
    /// Statics
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    /// Methods
    ////////////////////////////////////////////////////////////
    SceneGraphWindow::SceneGraphWindow(Monitor* monitor)
      : Parent{monitor, "Scene Graph Window", true}
    {
      show();
    }

    void SceneGraphWindow::updateMenuBar(sf::Time const&)
    {
    }

    void SceneGraphWindow::updateContentArea(sf::Time const&)
    {
      static ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_DefaultOpen;

      if (ImGui::TreeNodeEx("Scene Graph", tree_flags))
      {
        // Get model data.
        Frame::SceneNode frame_scene_graph;
        m_data_model->monitorState(*m_monitor, frame_scene_graph);

        // Draw data.
        uintmax_t id = 0;
        drawTreeSceneNode(frame_scene_graph, id);

        ImGui::TreePop();
      }
    }

    void SceneGraphWindow::drawTreeSceneNode(Frame::SceneNode const& frame_scene_node, uintmax_t& id) const
    {
      ++id;
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_DefaultOpen;

      if(frame_scene_node.m_children.empty())
        node_flags |= ImGuiTreeNodeFlags_Leaf;
      
      if (ImGui::TreeNodeEx(ImGuiUtil::labelize(frame_scene_node.category, id).c_str(), node_flags, "%s:%s (%lld)",frame_scene_node.type.c_str(), frame_scene_node.category.c_str(), id))
      {
        if (ImGui::IsItemClicked())
        {
        }
        for (std::size_t i = 0; i < frame_scene_node.m_children.size(); ++i)
        {
          drawTreeSceneNode(*frame_scene_node.m_children[i], id);
        }
        ImGui::TreePop();
      }
    }

  }
}