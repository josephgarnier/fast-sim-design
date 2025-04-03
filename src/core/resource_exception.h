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

#ifndef FAST_SIM_DESIGN_RESOURCE_EXCEPTION_H
#define FAST_SIM_DESIGN_RESOURCE_EXCEPTION_H

#include <exception>
#include <string>

namespace FastSimDesign {
class ResourceException final : public std::exception
{
private:
  using Parent = std::exception;

public:
  explicit ResourceException() = default;
  explicit ResourceException(std::string what) noexcept;
  ResourceException(ResourceException const&) = default;
  ResourceException(ResourceException&&) = default;
  ResourceException& operator=(ResourceException const&) = default;
  ResourceException& operator=(ResourceException&&) = default;
  virtual ~ResourceException() = default;

  virtual char const* what() const noexcept override;

private:
  std::string m_message{""};
};
} // namespace FastSimDesign
#endif