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
#include <imgui.h>

#include <cstdint>
#include <string>
#include <memory>

namespace FastSimDesign {
  namespace SimMonitor {
    class Monitorable;
    class Monitor;
    class Window
    {
    public:
      enum class ID : uint16_t
      {
        CONTROLLER,
        DEBUG,
        LOG,
        STATE_MACHINE,
        SCENE_GRAPH,
        TELEMETRY,

        ID_COUNT
      };

    public:
      using Ptr = std::unique_ptr<Window>;

    public:
      explicit Window(Monitor* monitor, std::string title, bool data_from_model, ImGuiWindowFlags flags = ImGuiWindowFlags_None) noexcept; // Default constructor
      Window(Window const&) = default; // Copy constructor
      Window(Window&&) = default; // Move constructor
      Window& operator=(Window const&) = default; // Copy assignment operator
      Window& operator=(Window&&) = default; // Move assignment operator
      virtual ~Window() = default; // Destructor

      bool isVisible() const noexcept;
      void setVisible(bool visible) noexcept;
      void switchVisibility() noexcept;
      void show() noexcept;
      void hide() noexcept;
      std::string const& getTitle() const noexcept;

      void setDataModel(Monitorable* data_model) noexcept;
      void unsetDataModel() noexcept;
      void update(sf::Time const& dt);

    private:
      void updateInterface(sf::Time const& dt);
      virtual void updateMenuBar(sf::Time const& dt) = 0;
      virtual void updateContentArea(sf::Time const& dt) = 0;

    protected:
      Monitor* m_monitor;
      Monitorable* m_data_model{nullptr};
      std::string m_title;
      bool m_open{false};
      ImGuiWindowFlags m_flags;

    private:
      bool m_visible{false};
      bool m_data_from_model;
    };
  }
}
#endif