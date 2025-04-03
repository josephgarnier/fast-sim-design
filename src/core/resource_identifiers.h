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

namespace sf {
class Texture;
class Font;
class Shader;
class SoundBuffer;
} // namespace sf

namespace FastSimDesign {
namespace Textures {
enum class ID : uint16_t
{
  ENTITIES,
  JUNGLE,
  TITLE_SCREEN,
  BUTTONS,
  EXPLOSION,
  PARTICLE,
  FINISH_LINE,
};
}

namespace Shaders {
enum class ID : uint16_t
{
  BRIGHTNESS_PASS,
  DOWN_SAMPLE_PASS,
  GAUSSIAN_BLUR_PASS,
  ADD_PASS,
};
}

namespace Fonts {
enum class ID : uint16_t
{
  MAIN,
};
}

namespace SoundEffect {
enum class ID : uint16_t
{
  ALLIED_GUN_FIRE,
  ENEMY_GUN_FIRE,
  EXPLOSION_1,
  EXPLOSION_2,
  LAUNCH_MISSILE,
  COLLECT_PICKUP,
  BUTTON,
};
}

namespace Music {
enum class ID : uint16_t
{
  MENU_THEME,
  MISSION_THEME,
};
}

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;
using ShaderHolder = ResourceHolder<sf::Shader, Shaders::ID>;
// using SoundBufferHolder = ResourceHolder<sf::Font, Fonts::ID>;
} // namespace FastSimDesign
#endif