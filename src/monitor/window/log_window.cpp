////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "log_window.h"
#include "../../core/log.h"
#include "../../core/log_imgu_sink.h"
#include "../../utils/generic_utility.h"

#include <imgui.h>
#include <spdlog/common.h>

namespace FastSimDesign {

  namespace SimMonitor {
    LogWindow::LogWindow(Monitor* monitor)
      : Parent{monitor, "Log Window", false}
    {
      backupFilterState();
      clearLogBuffers();
      Log::registerSink<FastSimDesign::imgui_sink_mt>(this);
      show();
    }

    void LogWindow::updateMenuBar(sf::Time const&)
    {
    }

    /// This function is inspired from \link https://gist.github.com/Lima-X/73f2bbf9ac03818ab8ef42ab15d09935 \endlink and from \link https://github.com/ocornut/imgui/blob/6982ce43f5b143c5dce5fab0ce07dd4867b705ae/imgui_demo.cpp#L8683 \endlink.
    void LogWindow::updateContentArea(sf::Time const&)
    {
      backupFilterState();

      // Draw options submenu.
      if (ImGui::BeginPopup("Options"))
      {
        ImGui::Checkbox("Auto-scroll", &m_enable_auto_scrolling);
        ImGui::EndPopup();
      }

      // Draw main window.
      drawButton("Options", [] {
        ImGui::OpenPopup("Options");
      });
      drawButton("Copy", [] {
        ImGui::LogToClipboard();
      });
      drawButton("Clear", [this] {
        clearLogBuffers();
      });
      drawLogFilter();
      ImGui::Separator();

      // Draw log view.
      if (ImGui::BeginChild("LogTextView", ImVec2{0, 0}, ImGuiChildFlags_None, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar))
      {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
        if (isFilterModified())
          rebuildFilteredView();
        drawLogLines();
        ImGui::PopStyleVar();

        // Handle auto-scrolling.
        if (m_enable_auto_scrolling && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
          ImGui::SetScrollHereY(1.0f);
      }
      ImGui::EndChild();
    }

    void LogWindow::backupFilterState()
    {
      m_previous_filter_state.log_level_filter = m_log_level_filter;
      memcpy(m_previous_filter_state.text_filter, m_text_filter.InputBuf, sizeof(m_previous_filter_state));
    }

    void LogWindow::drawButton(char const* label, std::function<void()> const& action) const
    {
      if (ImGui::Button(label))
        action();
      ImGui::SameLine();
    }

    void LogWindow::drawLogFilter()
    {
      // Filter out physical messages through logger.
      ImVec2 window_size = ImGui::GetWindowSize();
      [[maybe_unused]] ImVec2 outer_size = ImVec2{0.0f, window_size.y - (ImGui::GetFontSize() + ImGui::GetFrameHeight() * 2)};
      static char const* log_levels[] = {
        SPDLOG_LEVEL_NAME_TRACE.data(),
        SPDLOG_LEVEL_NAME_DEBUG.data(),
        SPDLOG_LEVEL_NAME_INFO.data(),
        SPDLOG_LEVEL_NAME_WARNING.data(),
        SPDLOG_LEVEL_NAME_ERROR.data(),
        SPDLOG_LEVEL_NAME_CRITICAL.data(),
        SPDLOG_LEVEL_NAME_OFF.data()};
      static auto const log_selection_width = []() -> float {
        float longest_text_width = 0.f;
        for (auto log_level_text : log_levels)
        {
          auto text_width = ImGui::CalcTextSize(log_level_text).x;
          if (text_width > longest_text_width)
            longest_text_width = text_width;
        }

        return longest_text_width + ImGui::GetStyle().FramePadding.x * 2.f + ImGui::GetFrameHeight();
      }();

      auto combo_box_right_alignment = window_size.x - (log_selection_width + ImGui::GetStyle().WindowPadding.x);
      ImGui::Text("Filter:");
      ImGui::SameLine();
      m_text_filter.Draw("##TextFilter", -(log_selection_width + ImGui::GetStyle().WindowPadding.x * 2.f + ImGui::GetStyle().FramePadding.x));
      ImGui::SetNextItemWidth(log_selection_width);
      ImGui::SameLine(combo_box_right_alignment);
      ImGui::Combo("##LevelFilter", toUnderlyingType<spdlog::level::level_enum>(&m_log_level_filter), log_levels, sizeof(log_levels) / sizeof(log_levels[0]));
    }

    bool LogWindow::isFilterModified() const
    {
      const auto& [p_log_level, p_text_filter] = m_previous_filter_state;

      bool filters_modified = false;
      filters_modified |= p_log_level != m_log_level_filter;
      filters_modified |= memcmp(p_text_filter, m_text_filter.InputBuf, sizeof(p_text_filter));

      return filters_modified;
    }

    void LogWindow::rebuildFilteredView()
    {
      m_filtered_view.resize(0);
      for (int line_number = 0; line_number < m_log_metadata.Size; ++line_number)
      {
        LogData const& log_entry = m_log_metadata[line_number];
        if (!doesPassFilter(std::make_pair(line_number, log_entry)))
          continue;

        m_filtered_view.push_back(line_number);
      }
    }

    bool LogWindow::doesPassFilter(std::pair<int, LogData const&> log_metadata) const
    {
      auto isPassLevelFilter = [this](spdlog::level::level_enum level) {
        return m_log_level_filter == spdlog::level::off || level == m_log_level_filter;
      };
      auto isPassTextFilter = [this](char const* line_start, char const* line_end) {
        return !m_text_filter.IsActive() || m_text_filter.PassFilter(line_start, line_end);
      };

      const auto& [log_index, log_entry] = log_metadata;
      if (!isPassLevelFilter(log_entry.m_level))
        return false;

      char const* line_start = m_log_text.begin() + log_entry.m_line_offset;
      char const* line_end = (log_index + 1 < m_log_metadata.Size) ? (m_log_text.begin() + m_log_metadata[log_index + 1].m_line_offset - 1) : m_log_text.end();
      if (!isPassTextFilter(line_start, line_end))
        return false;

      return true;
    }

    void LogWindow::drawLogLines() const
    {
      char const* buf = m_log_text.begin();
      char const* buf_end = m_log_text.end();
      ImGuiListClipper clipper{};

      clipper.Begin(m_filtered_view.Size);
      while (clipper.Step())
      {
        for (int clipper_line_number = clipper.DisplayStart; clipper_line_number < clipper.DisplayEnd; ++clipper_line_number)
        {
          int32_t const log_index = m_filtered_view[clipper_line_number];
          LogData const& log_entry = m_log_metadata[log_index];

          char const* line_start = buf + log_entry.m_line_offset;
          char const* line_end = (log_index + 1 < m_log_metadata.Size) ? (buf + m_log_metadata[log_index + 1].m_line_offset - 1) : buf_end;
          ImGui::TextUnformatted(line_start, line_end);
        }
      }
      clipper.End();
    }

    void LogWindow::clearLogBuffers()
    {
      m_log_text.clear();
      m_log_metadata.clear();
      m_filtered_view.clear();
    }

    void LogWindow::addLog(LogMessage message)
    {
      int old_size = m_log_text.size();
      m_log_text.append(message.m_message.c_str(), message.m_message.c_str() + message.m_message.size());
      m_log_metadata.push_back({message.m_level, old_size});
      if (doesPassFilter(std::make_pair(m_log_metadata.size() - 1, m_log_metadata.back())))
        m_filtered_view.push_back(m_log_metadata.size() - 1);
    }
  }
}