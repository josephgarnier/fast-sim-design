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

#ifndef FAST_SIM_DESIGN_WINDOW_H
#define FAST_SIM_DESIGN_WINDOW_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

namespace FastSimDesign {
  class ImGuiLayer;
  class Window
  {
  public:
    explicit Window(ImGuiLayer* imgui_layer, std::string title) noexcept; // Default constructor
    Window(Window const&) = default; // Copy constructor
    Window(Window&&) = default; // Move constructor
    Window& operator=(Window const&) = default; // Copy assignment operator
    Window& operator=(Window&&) = default; // Move assignment operator
    virtual ~Window() = default; // Destructor

    bool isOpen() const noexcept { return m_open; }
    void close() noexcept;
    bool isVisible() const noexcept { return m_visible; }
    void setVisible(bool visible) noexcept { m_visible = visible; }
    void show() noexcept { m_visible = true; }
    void hide() noexcept { m_visible = false; }
    std::string const& getTitle() const noexcept { return m_title; }

    void updateWindow(sf::RenderWindow& window, sf::Time const& dt);
  private:
    virtual void draw(sf::RenderWindow& window, sf::Time const& dt) noexcept = 0;

  protected:
    ImGuiLayer* m_imgui_layer;
    std::string m_title;
    bool m_visible = false;
    bool m_open = false;
  };
}
#endif