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

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  ModalDialog::ModalDialog(std::string title)
    : m_title{std::move(title)}
  {
  }

  void ModalDialog::close() noexcept
  {
    ImGui::CloseCurrentPopup();
    m_open = false;
  }

  void ModalDialog::updateModal(sf::Time const& dt) noexcept
  {
    if (m_open)
    {
      draw(dt);
    }
  }
}