////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "aircraft.h"
#include "category.h"
#include "../utils/generic_utility.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace FastSimDesign {
  Textures::ID Aircraft::toTextureID(Aircraft::Type type)
  {
    switch (type)
    {
      case Aircraft::Type::EAGLE:
        return Textures::ID::EAGLE;
      case Aircraft::Type::RAPTOR:
        return Textures::ID::RAPTOR;
    }
    return Textures::EAGLE;
  }

  Aircraft::Aircraft(Aircraft::Type type, TextureHolder const& textures) noexcept
    : m_type{type}
    , m_sprite{textures.get(Aircraft::toTextureID(type))}
  {
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  }

  unsigned int Aircraft::getCategory() const noexcept
  {
    switch (m_type)
    {
      case Aircraft::Type::EAGLE:
        return toUnderlyingType(Category::Type::PLAYER_AIRCRAFT);
      default:
        return toUnderlyingType(Category::Type::ENEMY_AIRCRAFT);
    }
  }

  void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const noexcept
  {
    target.draw(m_sprite, states);
  }
}