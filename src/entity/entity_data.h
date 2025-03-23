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

#include <SFML/Graphics/Rect.hpp>
#ifndef FAST_SIM_DESIGN_ENTITY_DATA_H
#define FAST_SIM_DESIGN_ENTITY_DATA_H

#include "../core/resource_identifiers.h"
#include "aircraft.h"
#include "projectile.h"
#include "pickup.h"
#include "particle.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include <functional>

namespace FastSimDesign {
  struct Direction
  {
    explicit Direction(float angle_, float distance_) noexcept; // Default constructor
    Direction(Direction const&) = default; // Copy constructor
    Direction(Direction&&) = default; // Move constructor
    Direction& operator=(Direction const&) = default; // Copy assignment operator
    Direction& operator=(Direction&&) = default; // Move assignment operator
    virtual ~Direction() = default; // Destructor

    float angle;
    float distance;
  };

  struct AircraftData
  {
    int hit_point;
    float speed;
    Textures::ID texture;
    sf::IntRect m_texture_rect;
    sf::Time fire_interval;
    std::vector<Direction> directions;
    bool has_rool_animation;
  };

  struct ProjectileData
  {
    int damage;
    float speed;
    Textures::ID texture;
    sf::IntRect m_texture_rect;
  };

  struct PickupData
  {
    std::function<void(Aircraft&)> action;
    Textures::ID texture;
    sf::IntRect m_texture_rect;
  };

  struct ParticleData
  {
    sf::Color m_color;
    sf::Time m_lifetime;
  };

  std::unordered_map<Aircraft::Type, AircraftData> initializeAircraftData();
  std::unordered_map<Projectile::Type, ProjectileData> initializeProjectileData();
  std::unordered_map<Pickup::Type, PickupData> initializePickupData();
  std::unordered_map<Particle::Type, ParticleData> initializeParticleData();
}
#endif