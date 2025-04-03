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

#ifndef FAST_SIM_DESIGN_IMGUI_UTIL_H
#define FAST_SIM_DESIGN_IMGUI_UTIL_H

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

namespace FastSimDesign {
namespace ImGuiUtil {
inline std::string labelize(std::string_view label, uintmax_t id)
{
  return std::format("{}##{}{}", label, label, id);
}
} // namespace ImGuiUtil
} // namespace FastSimDesign
#endif