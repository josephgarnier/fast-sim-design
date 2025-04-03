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
class CoreException final : public std::exception
{
private:
  using Parent = std::exception;

public:
  explicit CoreException() = default;
  explicit CoreException(std::string what) noexcept;
  CoreException(CoreException const&) = default;
  CoreException(CoreException&&) = default;
  CoreException& operator=(CoreException const&) = default;
  CoreException& operator=(CoreException&&) = default;
  virtual ~CoreException() = default;

  virtual char const* what() const noexcept override;

private:
  std::string m_message{""};
};
} // namespace FastSimDesign
#endif