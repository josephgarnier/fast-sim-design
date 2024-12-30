/******************************************************************************
 * Copyright 2024-present, Joseph Garnier
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 ******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_IMGU_SINK_H
#define FAST_SIM_DESIGN_IMGU_SINK_H

#include "../imgui_gui/window/log_window.h"
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <mutex>

namespace FastSimDesign {
  template<typename Mutex>
  class ImGuiSink : public spdlog::sinks::base_sink<Mutex>
  {
  private:
    using Parent = spdlog::sinks::base_sink<Mutex>;

  public:
    explicit ImGuiSink(LogWindow* log_view) noexcept
      : m_log_view{log_view}
    {
    }
    virtual ~ImGuiSink() = default; // Destructor

    void sink_it_(spdlog::details::log_msg const& message) override
    {
      spdlog::memory_buf_t formatted;
      Parent::formatter_->format(message, formatted);
      m_log_view->addLog(LogWindow::LogMessage{message.level, fmt::to_string(formatted)});
    }

    void flush_() override
    {
    }

  private:
    LogWindow* m_log_view;
  };

  // Using declarations.
  using imgui_sink_mt = ImGuiSink<std::mutex>;
  using imgui_sink_st = ImGuiSink<spdlog::details::null_mutex>;
}
#endif
