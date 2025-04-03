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

#ifndef FAST_SIM_DESIGN_MONITOR_H
#define FAST_SIM_DESIGN_MONITOR_H

#include "../core/configuration.h"
#include "modal/modal_dialog.h"
#include "monitor_exception.h"
#include "window/window.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui.h>

#include <map>

namespace FastSimDesign {
namespace SimMonitor {
class Monitor final
{
public:
  explicit Monitor() noexcept;
  Monitor(Monitor const&) = default;
  Monitor(Monitor&&) = default;
  Monitor& operator=(Monitor const&) = default;
  Monitor& operator=(Monitor&&) = default;
  virtual ~Monitor() = default;

  template<typename T>
  T& getWindow(Window::ID window_id)
  {
    auto found = m_windows.find(window_id);
    if (found == m_windows.end())
      throw MonitorException{"Monitor::getWindow - Window not found"};
    return static_cast<T&>(*found->second);
  }

  /// Method to initialize Dear ImGui context. Could be overridden to do custom
  /// Dear ImGui setup before application start.
  /// @param config The configuration object with basic application information.
  /// @param window The SFML window.
  virtual void initImGui(
      Configuration const& config, sf::RenderWindow& window) const;

  void handleEvent(sf::RenderWindow& window, sf::Event const& event) const;
  void updateImGui(sf::RenderWindow& window, sf::Time const& dt);
  void renderImGui(sf::RenderWindow& window);

  /// Method to destroy Dear ImGui context.
  virtual void disposeImGui();

  void openModalDialog(ModalDialog::Ptr&& modal_dialog);

protected:
private:
  template<typename T>
  T& createWindow(Window::ID window_id)
  {
    static_assert(std::is_base_of_v<Window, T>, "T must derive from Window");
    auto [it, inserted] =
        m_windows.emplace(window_id, std::make_unique<T>(this));
    return static_cast<T&>(*it->second);
  }

  void setSpectrumStyle() const;
  void createDockSpace() const;
  ImGuiID configureDockSpace() const;
  void configureDockNodes(ImGuiID dockspace_id) const;

private:
  std::map<Window::ID, Window::Ptr> m_windows{};
  std::vector<ModalDialog::Ptr> m_open_modal_dialogs{};
};
} // namespace SimMonitor
} // namespace FastSimDesign
#endif