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

#ifndef FAST_SIM_DESIGN_PICKUP_H
#define FAST_SIM_DESIGN_PICKUP_H

#include "entity.h"
#include "../core/resource_identifiers.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace FastSimDesign {
  class Aircraft;
  class Pickup : public Entity
  {
  public:
    enum class Type
    {
      HEALTH_REFILL,
      MISSILE_REFILL,
      FIRE_SPREAD,
      FIRE_RATE,
      TYPE_COUNT
    };

  private:
    using Parent = Entity;

  public:
    explicit Pickup(Pickup::Type type, TextureHolder const& textures) noexcept; // Default constructor
    virtual ~Pickup() = default; // Destructor

    virtual BitFlags<Category::Type> getCategory() const noexcept override;
    virtual sf::FloatRect getBoundingRect() const noexcept override;
    
    void apply(Aircraft& player) const;
  protected:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    Pickup::Type m_type;
    sf::Sprite m_sprite;
  };
}
#endif