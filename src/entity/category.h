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

namespace FastSimDesign {
  namespace Category {
    enum class Type : unsigned int
    {
      NONE = 0,
      SCENE = 1 << 0,
      PLAYER_AIRCRAFT = 1 << 1,
      ALLIED_AIRCRAFT = 1 << 2,
      ENEMY_AIRCRAFT = 1 << 3,
      ALL = NONE | SCENE | PLAYER_AIRCRAFT | ALLIED_AIRCRAFT | ENEMY_AIRCRAFT,
    };
  }
}
#endif