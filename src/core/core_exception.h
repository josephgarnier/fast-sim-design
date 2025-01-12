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

#ifndef FAST_SIM_DESIGN_CORE_EXCEPTION_H
#define FAST_SIM_DESIGN_CORE_EXCEPTION_H

#include <exception>
#include <string>

namespace FastSimDesign {
  class CoreException : public std::exception
  {
  private:
    using Parent = std::exception;

  public:
    explicit CoreException() = default; // Default constructor
    explicit CoreException(std::string what) noexcept; // Default constructor
    CoreException(CoreException const&) = default; // Copy constructor
    CoreException(CoreException&&) = default; // Move constructor
    CoreException& operator=(CoreException const&) = default; // Copy assignment operator
    CoreException& operator=(CoreException&&) = default; // Move assignment operator
    virtual ~CoreException() = default; // Destructor

    virtual char const* what() const noexcept override;

  public:
  protected:
  private:
    std::string m_message;
  };
}
#endif