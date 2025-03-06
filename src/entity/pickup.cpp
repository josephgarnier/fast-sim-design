////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "pickup.h"
#include "category.h"
#include "entity_data.h"
#include "../utils/sfml_util.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <unordered_map>

namespace FastSimDesign {
  namespace {
    std::unordered_map<Pickup::Type, PickupData> Data_Table = initializePickupData();
  }

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  Pickup::Pickup(Pickup::Type type, TextureHolder const& textures) noexcept
    : Parent{1}
    , m_type{type}
    , m_sprite{textures.get(Data_Table[type].texture)}
  {
    SFML::centerOrigin(m_sprite);
  }

  Category::Type Pickup::getCategory() const noexcept
  {
    return Category::Type::PICKUP;
  }

  sf::FloatRect Pickup::getBoundingRect() const noexcept
  {
    return getWorldTransform().transformRect(m_sprite.getGlobalBounds());
  }

  void Pickup::apply(Aircraft& player) const
  {
    Data_Table[m_type].action(player);
  }

  void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
  {
    target.draw(m_sprite, states);
  }

}