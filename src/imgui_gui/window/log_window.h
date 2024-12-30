/******************************************************************************
 * Copyright 2024-present, Joseph Garnier
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 ******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_LOG_WINDOW_H
#define FAST_SIM_DESIGN_LOG_WINDOW_H

#include "window.h"

#include <functional>
#include <spdlog/common.h>
#include <imgui.h>

namespace FastSimDesign {
  class LogWindow : public Window
  {
  public:
    struct LogMessage
    {
      spdlog::level::level_enum m_level;
      std::string m_message;
      
    };
  private:
    struct LogData
    {
      spdlog::level::level_enum m_level;
      int64_t m_line_offset; // Index to lines offset ; positions (offsets) of line starts in text buffer 'm_log_text'.
    };
    struct FilterState
    {
      spdlog::level::level_enum log_level_filter{spdlog::level::level_enum::off};
      decltype(ImGuiTextFilter::InputBuf) text_filter{};
    };

  private:
    using Parent = Window;

  public:
    explicit LogWindow() noexcept; // Default constructor, throw GuiException
    LogWindow(LogWindow const&) = default; // Copy constructor
    LogWindow(LogWindow&&) = default; // Move constructor
    LogWindow& operator=(LogWindow const&) = default; // Copy assignment operator
    LogWindow& operator=(LogWindow&&) = default; // Move assignment operator
    virtual ~LogWindow() = default; // Destructor

    virtual void updateWindow(sf::RenderWindow& window, sf::Time const& dt) noexcept override;
    void addLog(LogMessage message) noexcept;

  private:
    void backupFilterState() noexcept;
    void clearLogBuffers() noexcept;
    void drawButton(char const * label, std::function<void()> const & action) const noexcept;
    void drawLogFilter() noexcept;
    bool isFilterModified() const noexcept;
    void rebuildFilteredView() noexcept;
    bool doesPassFilter(std::pair<int, LogData const &> log_metadata) const noexcept;
    void drawLogLines() const noexcept;
  private:
    bool m_show_window;
    ImGuiTextBuffer m_log_text;
    ImVector<LogData> m_log_metadata;
    ImVector<int32_t> m_filtered_view; // A filtered array of indexes into the m_log_metadata vector. It's built from filter criteria once any filter change.
    
    ImGuiTextFilter m_text_filter;
    spdlog::level::level_enum m_log_level_filter;
    FilterState m_previous_filter_state;
    bool m_enable_auto_scrolling; // Keep scrolling if already at the bottom.
  };

}
#endif
