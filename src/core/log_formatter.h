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

#ifndef FAST_SIM_DESIGN_LOG_FORMATTER_H
#define FAST_SIM_DESIGN_LOG_FORMATTER_H

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/formatter.h>

namespace FastSimeDesign {
class LogFormater final : public spdlog::formatter
{
private:
  using Parent = spdlog::formatter;

private:
  static std::array<fmt::text_style, spdlog::level::n_levels> m_colors;

public:
  explicit LogFormater() = default;
  LogFormater(LogFormater const&) = default;
  LogFormater(LogFormater&&) = default;
  LogFormater& operator=(LogFormater const&) = default;
  LogFormater& operator=(LogFormater&&) = default;
  virtual ~LogFormater() = default;

  virtual void format(
      spdlog::details::log_msg const& msg, spdlog::memory_buf_t& dest) override;
  virtual std::unique_ptr<spdlog::formatter> clone() const override;
};
} // namespace FastSimeDesign
#endif
