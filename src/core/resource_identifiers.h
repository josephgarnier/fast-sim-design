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
      DESERT,
      TITLE_SCREEN,
    };
  }
  namespace Fonts {
    enum class ID : uint16_t
    {
      MAIN,
    };
  }

  using TextureHolder = ResourceHolder<sf::Texture, FastSimDesign::Textures::ID>;
  using FontHolder = ResourceHolder<sf::Font, FastSimDesign::Fonts::ID>;
}
#endif