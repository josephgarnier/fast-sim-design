////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "window.h"

#include <imgui.h>

#include <cassert>

namespace FastSimDesign {

  Window::Window(Application* app, std::string title) noexcept
    : m_app{app}
    , m_title{std::move(title)}
  {
    assert(m_app != nullptr);
  }

  void Window::close() noexcept
  {
    m_visible = false;
    m_open = false;
  }

  void Window::updateWindow(sf::RenderWindow& window, sf::Time const& dt)
  {
    if (m_visible)
    {
      m_open = true;
      draw(window, dt);
      if (!m_open)
        close();
    } else
    {
      m_open = false;
    }
  }
}