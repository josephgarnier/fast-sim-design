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

#ifndef FAST_SIM_DESIGN_MONITOR_EXCEPTION_H
#define FAST_SIM_DESIGN_MONITOR_EXCEPTION_H

#include <exception>
#include <string>

namespace FastSimDesign {
class MonitorException final : public std::exception
{
private:
  using Parent = std::exception;

public:
  explicit MonitorException() = default;
  explicit MonitorException(std::string what) noexcept;
  MonitorException(MonitorException const&) = default;
  MonitorException(MonitorException&&) = default;
  MonitorException& operator=(MonitorException const&) = default;
  MonitorException& operator=(MonitorException&&) = default;
  virtual ~MonitorException() = default;

  virtual char const* what() const noexcept override;

private:
  std::string m_message{};
};
} // namespace FastSimDesign
#endif