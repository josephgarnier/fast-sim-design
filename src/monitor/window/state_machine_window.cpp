////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "state_machine_window.h"
#include "../monitorable.h"

#include <imgui.h>

#include <ranges>

namespace FastSimDesign {
  namespace SimMonitor {
    ////////////////////////////////////////////////////////////
    /// Statics
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    /// Methods
    ////////////////////////////////////////////////////////////
    StateMachineWindow::StateMachineWindow(Monitor* monitor) noexcept
      : Parent{monitor, "State Machine Window", true}
    {
      show();
    }

    void StateMachineWindow::updateMenuBar(sf::Time const&)
    {
    }

    void StateMachineWindow::updateContentArea(sf::Time const&)
    {
      static ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner | ImGuiTableFlags_RowBg;

      if (ImGui::BeginTable("StatesStack", 1, table_flags))
      {
        ImGui::TableSetupColumn("Current Stack");
        ImGui::TableHeadersRow();

        // Get model data.
        Frame::StateMachine frame_state_machine;
        m_data_model->monitorState(*m_monitor, frame_state_machine);

        // Draw data.
        for (auto const& state : std::views::reverse(frame_state_machine.states))
        {
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::TextUnformatted(state.name.data());
        }
        ImGui::EndTable();
      }
    }
  }
}