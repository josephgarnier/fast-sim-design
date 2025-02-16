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
#include "../../core/application.h"

#include <imgui.h>

#include <ranges>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Statics
  ////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  StateMachineWindow::StateMachineWindow(Application* app) noexcept
    : Parent{app, "State Machine Window"}
    , m_state_machine{&app->getStateMachine()}
  {
    assert(m_state_machine != nullptr);
    show();
  }

  void StateMachineWindow::draw(sf::RenderWindow& window, sf::Time const& dt)
  {
    if (ImGui::Begin(Parent::m_title.c_str(), &(Parent::m_open)))
    {
      static ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner | ImGuiTableFlags_RowBg;

      if (ImGui::BeginTable("StatesStack", 1, table_flags))
      {
        ImGui::TableSetupColumn("Current Stack");
        ImGui::TableHeadersRow();

        std::vector<State::Ptr> const& states_stack = m_state_machine->getStack();
        for (auto const& state : std::views::reverse(states_stack))
        {
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::TextUnformatted(state->getName().c_str());
        }
        ImGui::EndTable();
      }
    }
    ImGui::End();
  }

}