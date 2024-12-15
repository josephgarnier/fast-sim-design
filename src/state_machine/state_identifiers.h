/******************************************************************************
 * Copyright 2024-present, Joseph Garnier
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 ******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_STATE_IDENTIFIERS_H
#define FAST_SIM_DESIGN_STATE_IDENTIFIERS_H

namespace FastSimDesign {
  namespace States {
    enum class ID : int
    {
      NONE,
      TITLE,
      MENU,
      GAME,
      LOADING,
      PAUSE,
    };
  }
}
#endif