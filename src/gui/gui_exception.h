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

#ifndef FAST_SIM_DESIGN_GUI_EXCEPTION_H
#define FAST_SIM_DESIGN_GUI_EXCEPTION_H

#include <exception>
#include <string>

namespace FastSimDesign {
  class GuiException : public std::exception
  {
  private:
    using Parent = std::exception;

  public:
    explicit GuiException() = default; // Default constructor
    explicit GuiException(std::string what) noexcept; // Default constructor
    GuiException(GuiException const&) = default; // Copy constructor
    GuiException(GuiException&&) = default; // Move constructor
    GuiException& operator=(GuiException const&) = default; // Copy assignment operator
    GuiException& operator=(GuiException&&) = default; // Move assignment operator
    virtual ~GuiException() = default; // Destructor
  
    virtual char const* what() const noexcept override;
  
  public:
  protected:
  private:
    std::string m_message;
  };
}
#endif