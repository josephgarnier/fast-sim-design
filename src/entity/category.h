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

#ifndef FAST_SIM_DESIGN_CATEGORY_H
#define FAST_SIM_DESIGN_CATEGORY_H

#include <cstdint>

namespace FastSimDesign {
  namespace Category {
    enum class Type : uint16_t
    {
      NONE = 0,
      SCENE_AIR_LAYER = 1 << 0,
      PLAYER_AIRCRAFT = 1 << 1,
      ALLIED_AIRCRAFT = 1 << 2,
      ENEMY_AIRCRAFT = 1 << 3,
      PICKUP = 1 << 4,
      ALLIED_PROJECTILE = 1 << 5,
      ENEMY_PROJECTILE = 1 << 6,

      AIRCRAFT = PLAYER_AIRCRAFT | ALLIED_AIRCRAFT | ENEMY_AIRCRAFT,
      PROJECTILE = ALLIED_PROJECTILE | ENEMY_PROJECTILE,
    };

    inline constexpr Category::Type operator|(Category::Type left, Category::Type right) noexcept
    {
      return static_cast<Category::Type>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
    }

    inline constexpr Category::Type& operator|=(Category::Type& left, Category::Type right)
    {
      left = static_cast<Category::Type>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
      return left;
    }

  }
}
#endif