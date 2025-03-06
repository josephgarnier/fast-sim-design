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

#ifndef FAST_SIM_DESIGN_RESOURCE_IDENTIFIERS_H
#define FAST_SIM_DESIGN_RESOURCE_IDENTIFIERS_H

#include "resource_holder.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace FastSimDesign {
  namespace Textures {
    enum class ID : uint16_t
    {
      EAGLE,
      RAPTOR,
      AVENGER,
      BULLET,
      MISSILE,
      DESERT,
      HEALTH_REFILL,
      MISSILE_REFILL,
      FIRE_SPREAD,
      FIRE_RATE,
      TITLE_SCREEN,
      BUTTON_NORMAL,
      BUTTON_SELECTED,
      BUTTON_PRESSED
    };
  }
  namespace Fonts {
    enum class ID : uint16_t
    {
      MAIN,
    };
  }

  using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
  using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;
}
#endif