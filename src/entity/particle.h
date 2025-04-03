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

#ifndef FAST_SIM_DESIGN_PARTICLE_H
#define FAST_SIM_DESIGN_PARTICLE_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstdint>

namespace FastSimDesign {
struct Particle
{
  enum class Type : uint16_t
  {
    PROPELLANT,
    SMOKE,
    TYPE_COUNT
  };

  sf::Vector2f m_position;
  sf::Color m_color;
  sf::Time m_lifetime;
};
} // namespace FastSimDesign
#endif