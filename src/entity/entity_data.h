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
#include "particle.h"
#include "pickup.h"
#include "projectile.h"

#include <functional>

namespace FastSimDesign {
struct Direction
{
  explicit Direction(float angle_, float distance_) noexcept;
  Direction(Direction const&) = default;
  Direction(Direction&&) = default;
  Direction& operator=(Direction const&) = default;
  Direction& operator=(Direction&&) = default;
  virtual ~Direction() = default;

  float angle{0.f};
  float distance{0.f};
};

struct AircraftData
{
  int hit_point{0};
  float speed{0.f};
  Textures::ID texture{};
  sf::IntRect m_texture_rect{};
  sf::Time fire_interval{sf::Time::Zero};
  std::vector<Direction> directions{};
  bool has_rool_animation{false};
};

struct ProjectileData
{
  int damage{0};
  float speed{0.f};
  Textures::ID texture{};
  sf::IntRect m_texture_rect{};
};

struct PickupData
{
  std::function<void(Aircraft&)> action{};
  Textures::ID texture{};
  sf::IntRect m_texture_rect{};
};

struct ParticleData
{
  sf::Color m_color{};
  sf::Time m_lifetime{};
};

std::unordered_map<Aircraft::Type, AircraftData> initializeAircraftData();
std::unordered_map<Projectile::Type, ProjectileData> initializeProjectileData();
std::unordered_map<Pickup::Type, PickupData> initializePickupData();
std::unordered_map<Particle::Type, ParticleData> initializeParticleData();
} // namespace FastSimDesign
#endif