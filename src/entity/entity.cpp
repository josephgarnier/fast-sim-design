////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "entity.h"

namespace FastSimDesign {
  void Entity::setVelocity(sf::Vector2f velocity) noexcept
  {
    m_velocity = velocity;
  }

  void Entity::setVelocity(float vx, float vy) noexcept
  {
    m_velocity.x = vx;
    m_velocity.y = vy;
  }

  sf::Vector2f Entity::getVelocity() const noexcept
  {
    return m_velocity;
  }

  void Entity::accelerate(sf::Vector2f velocity) noexcept
  {
    m_velocity += velocity;
  }

  void Entity::accelerate(float vx, float vy) noexcept
  {
    m_velocity.x += vx;
    m_velocity.y += vy;
  }

  void Entity::updateCurrent(sf::Time const & dt) noexcept
  {
    move(m_velocity * dt.asSeconds());
  }

}