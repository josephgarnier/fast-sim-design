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
    explicit MonitorException() = default; // Default constructor
    explicit MonitorException(std::string what) noexcept; // Default constructor
    MonitorException(MonitorException const&) = default; // Copy constructor
    MonitorException(MonitorException&&) = default; // Move constructor
    MonitorException& operator=(MonitorException const&) = default; // Copy assignment operator
    MonitorException& operator=(MonitorException&&) = default; // Move assignment operator
    virtual ~MonitorException() = default; // Destructor

    virtual char const* what() const noexcept override;

  private:
    std::string m_message;
  };
}
#endif