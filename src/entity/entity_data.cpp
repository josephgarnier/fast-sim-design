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
    data[Aircraft::Type::EAGLE].texture = Textures::ID::EAGLE;

    data[Aircraft::Type::RAPTOR].hit_point = 20;
    data[Aircraft::Type::RAPTOR].speed = 80.f;
    data[Aircraft::Type::RAPTOR].texture = Textures::ID::RAPTOR;
    data[Aircraft::Type::RAPTOR].directions.push_back(Direction{45.f, 80.f});
    data[Aircraft::Type::RAPTOR].directions.push_back(Direction{-45.f, 160.f});
    data[Aircraft::Type::RAPTOR].directions.push_back(Direction{45.f, 80.f});
    data[Aircraft::Type::RAPTOR].fire_interval = sf::Time::Zero;

    data[Aircraft::Type::AVENGER].hit_point = 40;
    data[Aircraft::Type::AVENGER].speed = 50.f;
    data[Aircraft::Type::AVENGER].texture = Textures::ID::AVENGER;
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{45.f, 50.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{0.f, 50.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{-45.f, 100.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{0.f, 50.f});
    data[Aircraft::Type::AVENGER].directions.push_back(Direction{45.f, 50.f});
    data[Aircraft::Type::AVENGER].fire_interval = sf::seconds(2);

    return data;
  }

  std::unordered_map<Projectile::Type, ProjectileData> initializeProjectileData()
  {
    std::unordered_map<Projectile::Type, ProjectileData> data{toUnderlyingType(Projectile::Type::TYPE_COUNT)};

    data[Projectile::Type::ALLIED_BULLET].damage = 10;
    data[Projectile::Type::ALLIED_BULLET].speed = 300.f;
    data[Projectile::Type::ALLIED_BULLET].texture = Textures::ID::BULLET;

    data[Projectile::Type::ENEMY_BULLET].damage = 10;
    data[Projectile::Type::ENEMY_BULLET].speed = 300.f;
    data[Projectile::Type::ENEMY_BULLET].texture = Textures::ID::BULLET;

    data[Projectile::Type::MISSILE].damage = 200;
    data[Projectile::Type::MISSILE].speed = 150.f;
    data[Projectile::Type::MISSILE].texture = Textures::ID::MISSILE;

    return data;
  }

  std::unordered_map<Pickup::Type, PickupData> initializePickupData()
  {
    std::unordered_map<Pickup::Type, PickupData> data{toUnderlyingType(Pickup::Type::TYPE_COUNT)};

    data[Pickup::Type::HEALTH_REFILL].texture = Textures::ID::HEALTH_REFILL;
    data[Pickup::Type::HEALTH_REFILL].action = [](Aircraft& a) {
      a.repair(25);
    };

    data[Pickup::Type::MISSILE_REFILL].texture = Textures::ID::MISSILE_REFILL;
    data[Pickup::Type::MISSILE_REFILL].action = [](Aircraft& a) {
      a.collectMissiles(3);
    };

    data[Pickup::Type::FIRE_SPREAD].texture = Textures::ID::FIRE_SPREAD;
    data[Pickup::Type::FIRE_SPREAD].action = [](Aircraft& a) {
      a.increaseSpread();
    };

    data[Pickup::Type::FIRE_RATE].texture = Textures::ID::FIRE_RATE;
    data[Pickup::Type::FIRE_RATE].action = [](Aircraft& a) {
      a.increaseFireRate();
    };

    return data;
  }

}