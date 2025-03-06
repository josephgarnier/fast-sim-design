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

#ifndef FAST_SIM_DESIGN_GENERIC_UTILITY_H
#define FAST_SIM_DESIGN_GENERIC_UTILITY_H

#include <type_traits>

namespace FastSimDesign {
  /// Concatenates two numbers. For a comparison of different methods.
  /// @see https://stackoverflow.com/questions/12700497/how-to-concatenate-two-integers-in-c
  inline unsigned int concatenate(unsigned int x, unsigned int y) noexcept
  {
    unsigned int pow = 10;
    while (y >= pow)
      pow *= 10;
    return x * pow + y;
  }

  template<typename T>
  inline constexpr auto toUnderlyingType(T value) noexcept
  {
    static_assert(std::is_enum_v<T>, "T must be an enum.");
    return static_cast<std::underlying_type_t<T>>(value);
  }

  template<typename T>
  inline constexpr auto toUnderlyingType(T* ptr) noexcept
  {
    static_assert(std::is_enum_v<T>, "T must be an enum.");
    return reinterpret_cast<std::underlying_type_t<T>*>(ptr);
  }
}
#endif