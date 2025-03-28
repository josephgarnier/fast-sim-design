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
    explicit Configuration() = default; // Default constructor
    Configuration(Configuration const&) = default; // Copy constructor
    Configuration(Configuration&&) = default; // Move constructor
    Configuration& operator=(Configuration const&) = default; // Copy assignment operator
    Configuration& operator=(Configuration&&) = default; // Move assignment operator
    virtual ~Configuration() = default; // Destructor

    std::string title = "Fast-Sim-Design";
    uint32_t width = 1280;
    uint32_t height = 720;
    bool fullscreen = false;
    bool use_custom_style = false;
  };
}
#endif