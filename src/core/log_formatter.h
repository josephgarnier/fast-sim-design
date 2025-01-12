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

#include <spdlog/formatter.h>
#include <spdlog/fmt/bundled/color.h>

namespace FastSimeDesign {
  class LogFormater : public spdlog::formatter
  {
  private:
    static std::array<fmt::text_style, spdlog::level::n_levels> m_colors;

  public:
    explicit LogFormater() = default; // Default constructor
    LogFormater(LogFormater const&) = default; // Copy constructor
    LogFormater(LogFormater&&) = default; // Move constructor
    LogFormater& operator=(LogFormater const&) = default; // Copy assignment operator
    LogFormater& operator=(LogFormater&&) = default; // Move assignment operator
    virtual ~LogFormater() = default; // Destructor

    virtual void format(spdlog::details::log_msg const& msg, spdlog::memory_buf_t& dest) override;
    virtual std::unique_ptr<spdlog::formatter> clone() const override;
  };
}
#endif
