////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "gui_exception.h"

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
GuiException::GuiException(std::string what) noexcept
  : Parent{}
  , m_message(std::move(what))
{
}

char const* GuiException::what() const noexcept
{
  return m_message.c_str();
}
} // namespace FastSimDesign