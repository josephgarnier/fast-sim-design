////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "log_formatter.h"

#include <spdlog/common.h>
#include <spdlog/details/fmt_helper.h>
#include <spdlog/details/os.h>
#include <spdlog/fmt/bundled/base.h>
#include <spdlog/fmt/bundled/chrono.h>

namespace FastSimeDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////
std::array<fmt::text_style, spdlog::level::n_levels> LogFormater::m_colors =
    []() {
      std::array<fmt::text_style, spdlog::level::n_levels> colors{};
      colors[spdlog::level::trace] = fmt::fg(fmt::terminal_color::white);
      colors[spdlog::level::debug] = fmt::fg(fmt::terminal_color::cyan);
      colors[spdlog::level::info] = fmt::fg(fmt::terminal_color::green);
      colors[spdlog::level::warn] = fmt::fg(fmt::terminal_color::yellow);
      colors[spdlog::level::err] = fmt::fg(fmt::terminal_color::red);
      colors[spdlog::level::critical] =
          fmt::fg(fmt::terminal_color::bright_red);
      colors[spdlog::level::off] = fmt::fg(fmt::terminal_color::white);
      return colors;
    }();

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
void LogFormater::format(
    spdlog::details::log_msg const& msg, spdlog::memory_buf_t& dest)
{
  static auto const basename = [](const char* filename) -> char const* {
    // if the size is 2 (1 character + null terminator) we can use the more
    // efficient strrchr the branch will be elided by optimizations
    if (sizeof(spdlog::details::os::folder_seps) == 2)
    {
      const char* rv =
          std::strrchr(filename, spdlog::details::os::folder_seps[0]);
      return rv != nullptr ? rv + 1 : filename;
    }
    else
    {
      const std::reverse_iterator<const char*> begin(
          filename + std::strlen(filename));
      const std::reverse_iterator<const char*> end(filename);

      const auto it = std::find_first_of(
          begin,
          end,
          std::begin(spdlog::details::os::folder_seps),
          std::end(spdlog::details::os::folder_seps) - 1);
      return it != end ? it.base() : filename;
    }
  };

  // Format the message in the format "%^[%T] [%s::%!()] %v%$".
  auto const& msg_style = m_colors[static_cast<size_t>(msg.level)];
  spdlog::memory_buf_t msg_buffer;

  // Add [%T]: ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S.
  std::time_t log_time = std::chrono::system_clock::to_time_t(msg.time);
  std::tm log_time_local = fmt::localtime(log_time);
  fmt::format_to(std::back_inserter(msg_buffer), "[{:%T}] ", log_time_local);

  // Add [%s::%!()].
  spdlog::memory_buf_t msg_source_buffer;
  msg_source_buffer.push_back('[');
  fmt::format_to(
      std::back_inserter(msg_source_buffer),
      "{}",
      msg.source.filename ? basename(msg.source.filename) : "unknown_file");
  msg_source_buffer.push_back(':');
  msg_source_buffer.push_back(':');
  fmt::format_to(
      std::back_inserter(msg_source_buffer),
      "{}",
      msg.source.funcname ? msg.source.funcname : "unknown_function");
  msg_source_buffer.push_back('(');
  msg_source_buffer.push_back(')');
  msg_source_buffer.push_back(']');
  fmt::format_to(
      std::back_inserter(msg_buffer),
      "{:<37}",
      fmt::to_string(msg_source_buffer));

  // Add %v.
  spdlog::details::fmt_helper::append_string_view(msg.payload, msg_buffer);

  // Add style color depending of the log level.
  fmt::format_to(
      std::back_inserter(dest),
      msg_style,
      "{}\n",
      fmt::to_string(msg_buffer));
}

std::unique_ptr<spdlog::formatter> LogFormater::clone() const
{
  return spdlog::details::make_unique<LogFormater>();
}
} // namespace FastSimeDesign