////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "entity_data.h"
#include "../utils/generic_utility.h"

#include <SFML/System/Time.hpp>

#include <unordered_map>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Direction::Statics
  ////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////
  /// Direction::Methods
  ////////////////////////////////////////////////////////////
  Direction::Direction(float angle_, float distance_) noexcept
    : angle{angle_}
    , distance{distance_}
  {
  }

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  std::unordered_map<Aircraft::Type, AircraftData> initializeAircraftData()
  {
    std::unordered_map<Aircraft::Type, AircraftData> data{toUnderlyingType(Aircraft::Type::TYPE_COUNT)};
    data[Aircraft::Type::EAGLE].hit_point = 100;
    data[Aircraft::Type::EAGLE].speed = 200.f;
    data[Aircraft::Type::EAGLE].fire_interval = sf::seconds(1);
    data[Aircraft::Type::EAGLE].texture = Textures::ID::ENTITIES;
    data[Aircraft::Type::EAGLE].m_texture_rect = sf::IntRect{0, 0, 48, 64};
    data[Aircraft::Type::EAGLE].has_rool_animation = true;

    data[Aircraft::Type::RAPTOR].hit_point = 20;
    data[Aircraft::Type::RAPTOR].speed = 80.f;
    data[Aircraft::Type::RAPTOR].texture = Textures::ID::ENTITIES;
    data[Aircraft::Type::RAPTOR].m_texture_rect = sf::IntRect{144, 0, 84, 64};
    data[Aircraft::Type::RAPTOR].directions.push_back(Direction{45.f, 80.f});
    data[Aircraft::Type::RAPTOR].directions.push_back(Direction{-45.f, 160.f});
    data[Aircraft::Type::RAPTOR].directions.push_back(Direction{45.f, 80.f});
    data[Aircraft::Type::RAPTOR].fire_interval = sf::Time::Zero;
    data[Aircraft::Type::RAPTOR].has_rool_animation = false;

    data[Aircraft::Type::AVENGER].hit_point = 40;
    data[Aircraft::Type::AVENGER].speed = 50.f;
    data[Aircraft::Type::AVENGER].texture = Textures::ID::ENTITIES;
    data[Aircraft::Type::AVENGER].m_texture_rect = sf::IntRect{228, 0, 60, 59};
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{45.f, 50.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{0.f, 50.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{-45.f, 100.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{0.f, 50.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{45.f, 50.f});
    data[Aircraft::Type::AVENGER].fire_interval = sf::seconds(2);
    data[Aircraft::Type::AVENGER].has_rool_animation = false;

    return data;
  }

  std::unordered_map<Projectile::Type, ProjectileData> initializeProjectileData()
  {
    std::unordered_map<Projectile::Type, ProjectileData> data{toUnderlyingType(Projectile::Type::TYPE_COUNT)};

    data[Projectile::Type::ALLIED_BULLET].damage = 10;
    data[Projectile::Type::ALLIED_BULLET].speed = 300.f;
    data[Projectile::Type::ALLIED_BULLET].texture = Textures::ID::ENTITIES;
    data[Projectile::Type::ALLIED_BULLET].m_texture_rect = sf::IntRect{175, 64, 3, 14};

    data[Projectile::Type::ENEMY_BULLET].damage = 10;
    data[Projectile::Type::ENEMY_BULLET].speed = 300.f;
    data[Projectile::Type::ENEMY_BULLET].texture = Textures::ID::ENTITIES;
    data[Projectile::Type::ENEMY_BULLET].m_texture_rect = sf::IntRect{178, 64, 3, 14};

    data[Projectile::Type::MISSILE].damage = 200;
    data[Projectile::Type::MISSILE].speed = 150.f;
    data[Projectile::Type::MISSILE].texture = Textures::ID::ENTITIES;
    data[Projectile::Type::MISSILE].m_texture_rect = sf::IntRect{160, 64, 15, 32};

    return data;
  }

  std::unordered_map<Pickup::Type, PickupData> initializePickupData()
  {
    std::unordered_map<Pickup::Type, PickupData> data{toUnderlyingType(Pickup::Type::TYPE_COUNT)};

    data[Pickup::Type::HEALTH_REFILL].texture = Textures::ID::ENTITIES;
    data[Pickup::Type::HEALTH_REFILL].m_texture_rect = sf::IntRect{0, 64, 40, 40};
    data[Pickup::Type::HEALTH_REFILL].action = [](Aircraft& aircraft) {
      aircraft.repair(25);
    };

    data[Pickup::Type::MISSILE_REFILL].texture = Textures::ID::ENTITIES;
    data[Pickup::Type::MISSILE_REFILL].m_texture_rect = sf::IntRect{40, 64, 40, 40};
    data[Pickup::Type::MISSILE_REFILL].action = [](Aircraft& aircraft) {
      aircraft.collectMissiles(3);
    };

    data[Pickup::Type::FIRE_SPREAD].texture = Textures::ID::ENTITIES;
    data[Pickup::Type::FIRE_SPREAD].m_texture_rect = sf::IntRect{80, 64, 40, 40};
    data[Pickup::Type::FIRE_SPREAD].action = [](Aircraft& aircraft) {
      aircraft.increaseSpread();
    };

    data[Pickup::Type::FIRE_RATE].texture = Textures::ID::ENTITIES;
    data[Pickup::Type::FIRE_RATE].m_texture_rect = sf::IntRect{120, 64, 40, 40};
    data[Pickup::Type::FIRE_RATE].action = [](Aircraft& aircraft) {
      aircraft.increaseFireRate();
    };

    return data;
  }

  std::unordered_map<Particle::Type, ParticleData> initializeParticleData()
  {
    std::unordered_map<Particle::Type, ParticleData> data{toUnderlyingType(Particle::Type::TYPE_COUNT)};

    data[Particle::Type::PROPELLANT].m_color = sf::Color{255, 255, 50};
    data[Particle::Type::PROPELLANT].m_lifetime = sf::seconds(0.6f);

    data[Particle::Type::SMOKE].m_color = sf::Color{50, 50, 50};
    data[Particle::Type::SMOKE].m_lifetime = sf::seconds(4.f);

    return data;
  }

}