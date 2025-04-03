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

#include <cassert>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
Entity::Entity(int hitpoints) noexcept
  : Parent{}
  , m_hitpoints{hitpoints}
{
}

void Entity::setVelocity(sf::Vector2f velocity) noexcept
{
  m_velocity = velocity;
}

void Entity::setVelocity(float vx, float vy) noexcept
{
  m_velocity.x = vx;
  m_velocity.y = vy;
}

sf::Vector2f const& Entity::getVelocity() const noexcept
{
  return m_velocity;
}

void Entity::accelerate(sf::Vector2f const& velocity) noexcept
{
  m_velocity += velocity;
}

void Entity::accelerate(float vx, float vy) noexcept
{
  m_velocity.x += vx;
  m_velocity.y += vy;
}

int Entity::getHitpoints() const noexcept
{
  return m_hitpoints;
}

void Entity::repair(int points)
{
  assert(points > 0);
  m_hitpoints += points;
}

void Entity::damage(int points)
{
  assert(points > 0);
  m_hitpoints -= points;
}

void Entity::destroy()
{
  m_hitpoints = 0;
}

bool Entity::isDestroyed() const noexcept
{
  return m_hitpoints <= 0;
}

void Entity::updateCurrent(sf::Time const& dt, CommandQueue&)
{
  move(m_velocity * dt.asSeconds());
}
} // namespace FastSimDesign