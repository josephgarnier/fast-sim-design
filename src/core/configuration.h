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

#ifndef FAST_SIM_DESIGN_CONFIGURATION_H
#define FAST_SIM_DESIGN_CONFIGURATION_H

#include <cstdint>
#include <string>

namespace FastSimDesign {
  struct Configuration
  {
    const std::string title = "Fast-Sim-Design";
    const uint32_t width = 1280;
    const uint32_t height = 720;
    const bool fullscreen = false;
    const bool use_custom_style = false;
  };
}
#endif