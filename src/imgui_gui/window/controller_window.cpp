////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "controller_window.h"
#include "../imgui_layer.h"
#include "../modal/about_dialog.h"

#include <iostream>
#include <memory>
#include <ostream>

#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_demo.cpp>

namespace FastSimDesign {
  ControllerWindow::ControllerWindow(ImGuiLayer* imgui_layer) noexcept
    : Parent{imgui_layer, "Controller Window"}
    , m_show_entity_inspector{true}
    , m_show_debug_window{true}
    , m_show_state_machine_window{true}
    , m_show_imgui_demo{false}
  {
    show();
    m_imgui_layer->getLogWindow().show();
  }


void ControllerWindow::draw(sf::RenderWindow& window, sf::Time const& dt) noexcept 
  {
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

    ImGui::Begin(Parent::m_title.c_str(), nullptr, window_flags);
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Open"))
        {
          std::cout << "Open selected" << std::endl;
        }
        if (ImGui::MenuItem("Save"))
        {
          std::cout << "Save selected" << std::endl;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
        {
          std::cout << "Exit selected" << std::endl;
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View"))
      {
        ImGui::MenuItem("Show Entity Inspector", "Ctrl+I", &m_show_entity_inspector);
        ImGui::MenuItem("Show Debug Window", "Ctrl+D", &m_show_debug_window);
        if (ImGui::MenuItem("Show Log Window", "Ctrl+L", m_imgui_layer->getLogWindow().isVisible()))
        {
          m_imgui_layer->getLogWindow().setVisible(!m_imgui_layer->getLogWindow().isVisible());
        }
        ImGui::MenuItem("Show State Machine Window", "Ctrl+M", &m_show_state_machine_window);
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Help"))
      {
        if (ImGui::MenuItem("About"))
          m_imgui_layer->openModalDialog(std::make_unique<AboutDialog>());
        ImGui::MenuItem("ImGui Demo", nullptr, &m_show_imgui_demo);
        ImGui::EndMenu();
      }

      ImGui::EndMenuBar();
    }

    if (m_show_imgui_demo)
      ImGui::ShowDemoWindow();
  
    ImGui::End();
  }
}