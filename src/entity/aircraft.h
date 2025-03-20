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

#ifndef FAST_SIM_DESIGN_AIRCRAFT_H
#define FAST_SIM_DESIGN_AIRCRAFT_H

#include "projectile.h"
#include "../core/resource_identifiers.h"
#include "../core/command.h"

#include <SFML/Graphics/Sprite.hpp>

namespace FastSimDesign {
  class TextNode;
  class Aircraft : public Entity
  {
  public:
    enum class Type : uint16_t
    {
      EAGLE,
      RAPTOR,
      AVENGER,
      TYPE_COUNT
    };

  private:
    using Parent = Entity;

  public:
    explicit Aircraft(Aircraft::Type type, TextureHolder const& textures, FontHolder const& fonts); // Default constructor
    virtual ~Aircraft() = default; // Destructor

    virtual BitFlags<Category::Type> getCategory() const noexcept override;
    virtual sf::FloatRect getBoundingRect() const noexcept override;
    virtual bool isMarkedForRemoval() const noexcept override;
    float getMaxSpeed() const noexcept;
    bool isAllied() const noexcept;

    void increaseFireRate() noexcept;
    void increaseSpread() noexcept;
    void collectMissiles(uint16_t count) noexcept;
    
    void fire() noexcept;
    void launchMissile() noexcept;

  private:
    void createBullets(SceneNode& node, TextureHolder const& textures) const noexcept;
    void createProjectile(SceneNode& node, Projectile::Type type, float x_offset, float y_offset, TextureHolder const& textures) const noexcept;
    void createPickup(SceneNode& node, TextureHolder const& textures) const noexcept;

    virtual void updateCurrent(sf::Time const& dt, CommandQueue& commands) override;
    void updateMovementPattern(sf::Time const& dt) noexcept;
    void checkPickupDrop(CommandQueue& commands) noexcept;
    void checkProjectileLaunch(sf::Time const& dt, CommandQueue& commands) noexcept;
    void updateDisplayedTexts() noexcept;

    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    Aircraft::Type m_type;
    sf::Sprite m_sprite;

    Command m_fire_command;
    Command m_missile_command;
    Command m_drop_pickup_command;
    sf::Time m_fire_countdown;
    bool m_is_firing;
    bool m_is_launching_missile;
    bool m_is_marked_for_removal;

    int m_fire_rate_level;
    int m_spread_level;
    int m_missile_ammo;

    float m_travelled_distance;
    std::size_t m_direction_index;

    TextNode* m_health_display;
    TextNode* m_missile_display;
  };
}
#endif
