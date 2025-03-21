////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "projectile.h"
#include "emitter_node.h"
#include "entity_data.h"
#include "particle.h"
#include "../utils/sfml_util.h"
#include "../utils/math_util.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <glm/trigonometric.hpp>

#include <memory>

namespace FastSimDesign {
  namespace {
    std::unordered_map<Projectile::Type, ProjectileData> Data_Table = initializeProjectileData();
  }

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  Projectile::Projectile(Projectile::Type type, TextureHolder const& textures)
    : Parent{1}
    , m_type{type}
    , m_sprite{textures.get(Data_Table[type].texture)}
    , m_target_direction{}
  {
    SFML::centerOrigin(m_sprite);

    // Add particle system for missiles.
    if (isGuided())
    {
      std::unique_ptr<EmitterNode> smoke = std::make_unique<EmitterNode>(Particle::Type::SMOKE);
      smoke->setPosition(0.f, getBoundingRect().height / 2.f);
      attachChild(std::move(smoke));

      std::unique_ptr<EmitterNode> propellant = std::make_unique<EmitterNode>(Particle::Type::PROPELLANT);
      propellant->setPosition(0.f, getBoundingRect().height / 2.f);
      attachChild(std::move(propellant));
    }
  }

  void Projectile::guideTowards(sf::Vector2f position)
  {
    assert(isGuided());
    m_target_direction = Math::normalize(position - getWorldPosition());
  }

  bool Projectile::isGuided() const
  {
    return m_type == Projectile::Type::MISSILE;
  }

  BitFlags<Category::Type> Projectile::getCategory() const noexcept
  {
    if (m_type == Projectile::Type::ENEMY_BULLET)
      return BitFlags<Category::Type>{Category::Type::ENEMY_PROJECTILE};
    else
      return BitFlags<Category::Type>{Category::Type::ALLIED_PROJECTILE};
  }

  sf::FloatRect Projectile::getBoundingRect() const noexcept
  {
    return getWorldTransform().transformRect(m_sprite.getGlobalBounds());
  }

  float Projectile::getMaxSpeed() const
  {
    return Data_Table[m_type].speed;
  }

  int Projectile::getDamage() const
  {
    return Data_Table[m_type].damage;
  }

  void Projectile::updateCurrent(const sf::Time& dt, CommandQueue& commands)
  {
    if (isGuided())
    {
      // We apply an algorithm of "steering behaviors".
      float const approach_rate = 200.f;

      sf::Vector2f new_velocity = Math::normalize(approach_rate * dt.asSeconds() * m_target_direction + getVelocity());
      new_velocity *= getMaxSpeed();
      float angle = std::atan2(new_velocity.y, new_velocity.x);

      setRotation(glm::degrees(angle) + 90.f);
      setVelocity(new_velocity);
    }

    Parent::updateCurrent(dt, commands);
  }

  void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
  {
    target.draw(m_sprite, states);
  }

}