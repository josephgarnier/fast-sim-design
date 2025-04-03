////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "resource_exception.h"

namespace FastSimDesign {
ResourceException::ResourceException(std::string what) noexcept
  : Parent{}
  , m_message{std::move(what)}
{
}

const char* ResourceException::what() const noexcept
{
  return m_message.c_str();
}
} // namespace FastSimDesign