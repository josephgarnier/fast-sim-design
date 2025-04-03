////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "log.h"

#include "log_formatter.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////
std::shared_ptr<spdlog::logger> Log::s_logger{};

void Log::init()
{
  // See here for the list of supported sinks :
  // https://github.com/gabime/spdlog/wiki/4.-Sinks
  std::vector<spdlog::sink_ptr> log_sinks;
  log_sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  log_sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
      "fast-sim-design.log",
      false));

  auto formatter = std::make_unique<FastSimeDesign::LogFormater>();
  log_sinks[0]->set_formatter(
      std::move(formatter)); // console formatter to "%^[%T] [%s::%!()] %v%$"
  log_sinks[0]->set_level(spdlog::level::trace);
  log_sinks[1]->set_pattern("[%Y-%m-%d %T] [%l] [%s::%!()] %v"); // file pattern
  log_sinks[1]->set_level(spdlog::level::trace);

  Log::s_logger = std::make_shared<spdlog::logger>(
      "APP",
      std::begin(log_sinks),
      std::end(log_sinks));
  Log::s_logger->set_level(spdlog::level::trace);
  Log::s_logger->flush_on(spdlog::level::trace);
  spdlog::register_logger(Log::s_logger);
}

void Log::shutdown()
{
  spdlog::shutdown();
}

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
} // namespace FastSimDesign