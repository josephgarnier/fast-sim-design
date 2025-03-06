////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "text_node.h"
#include "../utils/sfml_util.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Statics
  ////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  TextNode::TextNode(FontHolder const& fonts, std::string text) noexcept
    : m_text{}
  {
    m_text.setFont(fonts.get(Fonts::ID::MAIN));
    m_text.setCharacterSize(20);
    setString(std::move(text));
  }

  void TextNode::setString(std::string text) noexcept
  {
    m_text.setString(std::move(text));
    SFML::centerOrigin(m_text);
  }

  void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
  {
    target.draw(m_text, states);
  }

}