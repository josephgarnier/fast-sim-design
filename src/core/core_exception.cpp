////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "core_exception.h"

namespace FastSimDesign {
CoreException::CoreException(std::string what) noexcept
  : Parent{}
  , m_message{std::move(what)}
{
}

char const* CoreException::what() const noexcept
{
  return m_message.c_str();
}
} // namespace FastSimDesign