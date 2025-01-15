////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#pragma once

#ifndef FAST_SIM_DESIGN_SPRITE_NODE_H
#define FAST_SIM_DESIGN_SPRITE_NODE_H

#include "scene_node.h"

#include <SFML/Graphics/Sprite.hpp>

namespace FastSimDesign {
  class SpriteNode : public SceneNode
  {
  private:
    using Parent = SceneNode;

  public:
    explicit SpriteNode(sf::Texture const & texture) noexcept; // Default constructor
    explicit SpriteNode(sf::Texture const & texture, sf::IntRect const & textureRect) noexcept;
    virtual ~SpriteNode() = default; // Destructor

  protected:
  private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  public:
  protected:
  private:
    sf::Sprite m_sprite;
  };
}
#endif