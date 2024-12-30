/******************************************************************************
 * Copyright 2024-present, Joseph Garnier
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 ******************************************************************************/

#pragma once

#ifndef FAST_SIM_DESIGN_WINDOW_H
#define FAST_SIM_DESIGN_WINDOW_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

namespace FastSimDesign {
  class Window
  {
  public:
    explicit Window(std::string title) noexcept; // Default constructor
    Window(Window const&) = default; // Copy constructor
    Window(Window&&) = default; // Move constructor
    Window& operator=(Window const&) = default; // Copy assignment operator
    Window& operator=(Window&&) = default; // Move assignment operator
    virtual ~Window() = default; // Destructor

    virtual void updateWindow(sf::RenderWindow& window, sf::Time const& dt) noexcept = 0;

  protected:
    std::string m_title;

  private:
  };
}
#endif