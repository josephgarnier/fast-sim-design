////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "about_dialog.h"

#include <imgui.h>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  AboutDialog::AboutDialog() noexcept
    : Parent{"About"}
  {
  }

  void AboutDialog::draw(sf::Time const&)
  {
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{0.5f, 0.5f});
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;

    if (ImGui::BeginPopupModal(Parent::m_title.c_str(), &(Parent::m_open), window_flags))
    {
      ImGui::Text("FastSimDesign");
      ImGui::Spacing();
      ImGui::Text("Version: 1.0.0");
      ImGui::Text("Author: Joseph Garnier");
      ImGui::Spacing();
      ImGui::Spacing();

      if (ImGui::Button("Close"))
        Parent::close();

      ImGui::EndPopup();
    }
  }
}