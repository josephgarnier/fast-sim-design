////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "label.h"

namespace FastSimDesign {
namespace GUI {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
Label::Label(std::string text, FontHolder const& fonts) noexcept
  : m_text{std::move(text), fonts.get(Fonts::ID::MAIN), 16}
{
}

bool Label::isSelectable() const noexcept
{
  return false;
}

void Label::setText(std::string const& text) noexcept
{
  m_text.setString(text);
}

void Label::handleEvent(sf::Event const&) noexcept {}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(m_text, states);
}
} // namespace GUI
} // namespace FastSimDesign