////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "sprite_node.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace FastSimDesign {
  SpriteNode::SpriteNode(sf::Texture const& texture) noexcept
    : Parent{}
    , m_sprite{texture}
  {
  }

  SpriteNode::SpriteNode(sf::Texture const& texture, const sf::IntRect& textureRect) noexcept
    : Parent{}
    , m_sprite{texture, textureRect}
  {
  }

  void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
  {
    target.draw(m_sprite, states);
  }
}