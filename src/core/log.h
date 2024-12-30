/******************************************************************************
 * Copyright 2024-present, Joseph Garnier
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 ******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_LOG_H
#define FAST_SIM_DESIGN_LOG_H

#define LOG_LEVEL_TRACE SPDLOG_LEVEL_TRACE
#define LOG_LEVEL_DEBUG SPDLOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO SPDLOG_LEVEL_INFO
#define LOG_LEVEL_WARN SPDLOG_LEVEL_WARN
#define LOG_LEVEL_ERROR SPDLOG_LEVEL_ERROR
#define LOG_LEVEL_CRITICAL SPDLOG_LEVEL_CRITICAL
#define LOG_LEVEL_OFF SPDLOG_LEVEL_OFF

#if !defined(LOG_ACTIVE_LEVEL)
#define LOG_ACTIVE_LEVEL LOG_LEVEL_INFO
#endif

#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>
#include <utility>

namespace FastSimDesign {
  class Log final
  {
  public:
    static void init();
    template<typename TSink, typename... TSinkArgs>
    static void registerSink(TSinkArgs&&... args)
    {
      spdlog::get("APP")->sinks().push_back(std::make_shared<TSink>(std::forward<TSinkArgs>(args)...));
      spdlog::get("APP")->sinks().back()->set_pattern("[%Y-%m-%d %T] [%l] [%s::%!()] %v"); // ImGui window
      spdlog::get("APP")->sinks().back()->set_level(spdlog::level::trace);
    }
    static void shutdown();
    static std::shared_ptr<spdlog::logger>& getLogger() noexcept
    {
      return Log::s_logger;
    }

  private:
    static std::shared_ptr<spdlog::logger> s_logger;

  public:
    explicit Log() = default; // Default constructor
    Log(Log const&) = default; // Copy constructor
    Log(Log&&) = default; // Move constructor
    Log& operator=(Log const&) = default; // Copy assignment operator
    Log& operator=(Log&&) = default; // Move assignment operator
    virtual ~Log() = default; // Destructor
  };
}

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_TRACE
#define LOG_TRACE(...) FastSimDesign::Log::getLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, __VA_ARGS__);
#else
#define LOG_TRACE(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(...) FastSimDesign::Log::getLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, __VA_ARGS__);
#else
#define LOG_DEBUG(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(...) FastSimDesign::Log::getLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, __VA_ARGS__);
#else
#define LOG_INFO(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN(...) FastSimDesign::Log::getLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, __VA_ARGS__);
#else
#define LOG_WARN(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(...) FastSimDesign::Log::getLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err, __VA_ARGS__);
#else
#define LOG_ERROR(...) (void)0
#endif

#if LOG_ACTIVE_LEVEL <= LOG_LEVEL_CRITICAL
#define LOG_CRITICAL(...) FastSimDesign::Log::getLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical, __VA_ARGS__);
#else
#define LOG_CRITICAL(...) (void)0
#endif

#endif