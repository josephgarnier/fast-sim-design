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

#ifndef FAST_SIM_DESIGN_MATH_UTIL_H
#define FAST_SIM_DESIGN_MATH_UTIL_H

#include <SFML/System/Vector2.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>

#include <type_traits>

namespace FastSimDesign {
  namespace Math {
    template<typename T>
    inline constexpr typename std::enable_if<std::is_floating_point<T>::value, sf::Vector2<T>>::type normalize(sf::Vector2<T> const& vector)
    {
      T length_squared = glm::length2(glm::vec<2, T>{vector.x, vector.y});

      if (length_squared > T{0})
      {
        glm::vec<2, T> unit_vector = glm::normalize(glm::vec<2, T>{vector.x, vector.y});
        return sf::Vector2<T>{unit_vector.x, unit_vector.y};
      }
      return sf::Vector2<T>{T{0}, T{0}};
    }

    inline float length(sf::Vector2f const& vector) noexcept
    {
      return glm::length(glm::vec2(vector.x, vector.y));
    }

    inline float distance(sf::Vector2f const& left, sf::Vector2f const& right)
    {
      return glm::distance(glm::vec2(left.x, left.y), glm::vec2(right.x, right.y));
    }

    inline int randomInt(int exclusive_max)
    {
      return glm::linearRand(0, exclusive_max - 1);
    }
  }
}
#endif