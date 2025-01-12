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

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "../gui/scene_node.h"

namespace FastSimDesign {
  class Entity : public SceneNode
  {
  public:
  private:
    using Parent = SceneNode;

  public:
    explicit Entity() = default; // Default constructor
    virtual ~Entity() = default; // Destructor

    void setVelocity(sf::Vector2f velocity) noexcept;
    void setVelocity(float vx, float vy) noexcept;
    sf::Vector2f getVelocity() const noexcept;

    void accelerate(sf::Vector2f velocity) noexcept;
    void accelerate(float vx, float vy) noexcept;

  protected:
  private:
    virtual void updateCurrent(sf::Time const & dt) noexcept override;

  public:
  protected:
  private:
    sf::Vector2f m_velocity;
  };
}
#endif