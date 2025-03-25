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

#include "../utils/generic_utility.h"
#include "../utils/bit_flags.h"

#include <ostream>
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
      PARTICLE_SYSTEM = 1 << 7,

      AIRCRAFT = PLAYER_AIRCRAFT | ALLIED_AIRCRAFT | ENEMY_AIRCRAFT,
      PROJECTILE = ALLIED_PROJECTILE | ENEMY_PROJECTILE,
    };

    inline std::string toString(uint16_t const& type)
    {
      // Using the underlying type avoids having to deal with cast errors,
      // before calling this function, which can occur for undeclared
      // compound values.
      switch (type)
      {
        case toUnderlyingType(Type::NONE): return "NONE";
        case toUnderlyingType(Type::SCENE_AIR_LAYER): return "SCENE_AIR_LAYER";
        case toUnderlyingType(Type::PLAYER_AIRCRAFT): return "PLAYER_AIRCRAFT";
        case toUnderlyingType(Type::ALLIED_AIRCRAFT): return "ALLIED_AIRCRAFT";
        case toUnderlyingType(Type::ENEMY_AIRCRAFT): return "ENEMY_AIRCRAFT";
        case toUnderlyingType(Type::PICKUP): return "PICKUP";
        case toUnderlyingType(Type::ALLIED_PROJECTILE): return "ALLIED_PROJECTILE";
        case toUnderlyingType(Type::ENEMY_PROJECTILE): return "ENEMY_PROJECTILE";
        case toUnderlyingType(Type::AIRCRAFT): return "AIRCRAFT";
        case toUnderlyingType(Type::PROJECTILE): return "PROJECTILE";
        default: return "UNKNOWN";
      }
    }

    template<typename T>
    inline std::string toString(BitFlags<T> const& flags)
    {
      using UnderlyingT = typename BitFlags<T>::UnderlyingT;
      UnderlyingT raw = flags.toRaw();

      return toString(raw);
    }

    inline std::ostream& operator<<(std::ostream& stream, Type const& left)
    {
      return stream << toString(toUnderlyingType(left));
    }

    inline constexpr Type operator|(Type left, Type right) noexcept
    {
      return static_cast<Type>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
    }

    inline constexpr Type& operator|=(Type& left, Type right)
    {
      left = static_cast<Type>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
      return left;
    }

  }
}
#endif