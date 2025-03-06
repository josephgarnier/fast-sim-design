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

#ifndef FAST_SIM_DESIGN_ENTITY_DATA_H
#define FAST_SIM_DESIGN_ENTITY_DATA_H

#include "../core/resource_identifiers.h"
#include "aircraft.h"
#include "projectile.h"
#include "pickup.h"

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
    explicit AircraftData() = default; // Default constructor
    AircraftData(AircraftData const&) = default; // Copy constructor
    AircraftData(AircraftData&&) = default; // Move constructor
    AircraftData& operator=(AircraftData const&) = default; // Copy assignment operator
    AircraftData& operator=(AircraftData&&) = default; // Move assignment operator
    virtual ~AircraftData() = default; // Destructor

    int hit_point;
    float speed;
    Textures::ID texture;
    sf::Time fire_interval;
    std::vector<Direction> directions;
  };

  struct ProjectileData
  {
    explicit ProjectileData() = default; // Default constructor
    ProjectileData(ProjectileData const&) = default; // Copy constructor
    ProjectileData(ProjectileData&&) = default; // Move constructor
    ProjectileData& operator=(ProjectileData const&) = default; // Copy assignment operator
    ProjectileData& operator=(ProjectileData&&) = default; // Move assignment operator
    virtual ~ProjectileData() = default; // Destructor

    int damage;
    float speed;
    Textures::ID texture;
  };

  struct PickupData
  {
    explicit PickupData() = default; // Default constructor
    PickupData(PickupData const&) = default; // Copy constructor
    PickupData(PickupData&&) = default; // Move constructor
    PickupData& operator=(PickupData const&) = default; // Copy assignment operator
    PickupData& operator=(PickupData&&) = default; // Move assignment operator
    virtual ~PickupData() = default; // Destructor

    std::function<void(Aircraft&)> action;
    Textures::ID texture;
  };

  std::unordered_map<Aircraft::Type, AircraftData> initializeAircraftData();
  std::unordered_map<Projectile::Type, ProjectileData> initializeProjectileData();
  std::unordered_map<Pickup::Type, PickupData> initializePickupData();
}
#endif