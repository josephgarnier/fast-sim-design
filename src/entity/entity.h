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

#ifndef FAST_SIM_DESIGN_ENTITY_H
#define FAST_SIM_DESIGN_ENTITY_H

#include "../gui/scene_node.h"

namespace FastSimDesign {
class Entity : public SceneNode
{
public:
private:
  using Parent = SceneNode;

public:
  explicit Entity(int hitpoints) noexcept;
  virtual ~Entity() = default;

  void setVelocity(sf::Vector2f velocity) noexcept;
  void setVelocity(float vx, float vy) noexcept;
  sf::Vector2f const& getVelocity() const noexcept;

  void accelerate(sf::Vector2f const& velocity) noexcept;
  void accelerate(float vx, float vy) noexcept;

  int getHitpoints() const noexcept;
  void repair(int points);
  void damage(int points);
  void destroy();
  virtual bool isDestroyed() const noexcept override;

protected:
  virtual void updateCurrent(
      sf::Time const& dt, CommandQueue& commands) override;

private:
  sf::Vector2f m_velocity{};
  int m_hitpoints{0};
};
} // namespace FastSimDesign
#endif