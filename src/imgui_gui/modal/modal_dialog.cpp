////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "modal_dialog.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  ModalDialog::ModalDialog(std::string title) noexcept
    : m_title{std::move(title)}
  {
  }

  void ModalDialog::close()
  {
    ImGui::CloseCurrentPopup();
    m_open = false;
  }

  void ModalDialog::updateModal(sf::Time const& dt)
  {
    if (m_open)
    {
      if (!ImGui::IsPopupOpen(m_title.c_str()))
      {
        ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None;
        ImGui::OpenPopup(m_title.c_str(), popup_flags);
      }
      draw(dt);
    }
  }
}