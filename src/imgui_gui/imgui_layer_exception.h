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

#ifndef FAST_SIM_DESIGN_VISUALIZER_EXCEPTION_H
#define FAST_SIM_DESIGN_VISUALIZER_EXCEPTION_H

#include <exception>
#include <string>

namespace FastSimDesign {
  class ImGuiLayerException : public std::exception
  {
  private:
    using Parent = std::exception;

  public:
    explicit ImGuiLayerException() = default; // Default constructor
    explicit ImGuiLayerException(std::string what) noexcept; // Default constructor
    ImGuiLayerException(ImGuiLayerException const&) = default; // Copy constructor
    ImGuiLayerException(ImGuiLayerException&&) = default; // Move constructor
    ImGuiLayerException& operator=(ImGuiLayerException const&) = default; // Copy assignment operator
    ImGuiLayerException& operator=(ImGuiLayerException&&) = default; // Move assignment operator
    virtual ~ImGuiLayerException() = default; // Destructor

    virtual char const* what() const noexcept override;

  public:
  protected:
  private:
    std::string m_message;
  };
}
#endif