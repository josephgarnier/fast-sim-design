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

#ifndef FAST_SIM_DESIGN_PROJECTILE_H
#define FAST_SIM_DESIGN_PROJECTILE_H

#include "../core/resource_identifiers.h"
#include "entity.h"

#include <SFML/Graphics/Sprite.hpp>

namespace FastSimDesign {
  class Projectile : public Entity
  {
  public:
    enum class Type : uint16_t
    {
      ALLIED_BULLET,
      ENEMY_BULLET,
      MISSILE,
      TYPE_COUNT
    };
    
  public:
  private:
    using Parent = Entity;

  public:
    explicit Projectile(Projectile::Type type, TextureHolder const & textures); // Default constructor
    virtual ~Projectile() = default; // Destructor
    
    void guideTowards(sf::Vector2f position);
    bool isGuided() const;
    
    virtual BitFlags<Category::Type> getCategory() const noexcept override;
    virtual sf::FloatRect getBoundingRect() const noexcept override;
    float getMaxSpeed() const;
    int getDamage() const;
  protected:
  private:
    virtual void updateCurrent(const sf::Time &dt, CommandQueue &commands) override;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    Projectile::Type m_type;
    sf::Sprite m_sprite;
    sf::Vector2f m_target_direction;
  };
}
#endif