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
  class ResourceException : public std::exception
  {
  private:
    using Parent = std::exception;

  public:
    explicit ResourceException() = default; // Default constructor
    explicit ResourceException(std::string what) noexcept; // Default constructor
    ResourceException(ResourceException const&) = default; // Copy constructor
    ResourceException(ResourceException&&) = default; // Move constructor
    ResourceException& operator=(ResourceException const&) = default; // Copy assignment operator
    ResourceException& operator=(ResourceException&&) = default; // Move assignment operator
    virtual ~ResourceException() = default; // Destructor

    virtual char const* what() const noexcept override;

  public:
  protected:
  private:
    std::string m_message;
  };
}
#endif