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

#include <cassert>

namespace FastSimDesign {
namespace SimMonitor {
Window::Window(
    Monitor* monitor,
    std::string title,
    bool data_from_model,
    ImGuiWindowFlags flags) noexcept
  : m_monitor{monitor}
  , m_title{std::move(title)}
  , m_flags{flags}
  , m_data_from_model{data_from_model}
{
  assert(monitor != nullptr);
}

bool Window::isVisible() const noexcept
{
  return m_visible;
}

void Window::setVisible(bool visible) noexcept
{
  m_visible = visible;
}

void Window::switchVisibility() noexcept
{
  m_visible = !m_visible;
}

void Window::show() noexcept
{
  m_visible = true;
}

void Window::hide() noexcept
{
  m_visible = false;
}

std::string const& Window::getTitle() const noexcept
{
  return m_title;
}

void Window::setDataModel(Monitorable* data_model) noexcept
{
  assert(data_model != nullptr);
  m_data_model = data_model;
}

void Window::unsetDataModel() noexcept
{
  m_data_model = nullptr;
}

void Window::update(sf::Time const& dt)
{
  if (m_visible)
  {
    m_open = true;
    updateInterface(dt);
    if (!m_open)
      hide();
  }
  else
  {
    m_open = false;
  }
}

void Window::updateInterface(sf::Time const& dt)
{
  if (ImGui::Begin(m_title.c_str(), &m_open, m_flags))
  {
    updateMenuBar(dt);
    if (!m_data_from_model || (m_data_from_model && m_data_model != nullptr))
      updateContentArea(dt);
  }
  ImGui::End();
}
} // namespace SimMonitor
} // namespace FastSimDesign