////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "component.h"

namespace FastSimDesign {
namespace GUI {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
bool Component::isSelected() const noexcept
{
  return m_is_selected;
}

void Component::select() noexcept
{
  m_is_selected = true;
}

void Component::deselect() noexcept
{
  m_is_selected = false;
}

bool Component::isActive() const noexcept
{
  return m_is_active;
}

void Component::activate() noexcept
{
  m_is_active = true;
}

void Component::deactivate() noexcept
{
  m_is_active = false;
}

} // namespace GUI
} // namespace FastSimDesign