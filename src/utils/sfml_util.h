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

#ifndef FAST_SIM_DESIGN_RENDER_UTIL_H
#define FAST_SIM_DESIGN_RENDER_UTIL_H

#include <SFML/Graphics/Transformable.hpp>

#include <cmath>

namespace FastSimDesign {

  template<typename T>
  inline void centerOrigin(T& ui_transformable)
  {
    sf::FloatRect bounds = ui_transformable.getLocalBounds();
    ui_transformable.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
  }
}
#endif