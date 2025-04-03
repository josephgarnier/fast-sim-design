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

#include "../modal/about_dialog.h"
#include "../monitor.h"
#include "log_window.h"
#include "scene_graph_window.h"
#include "state_machine_window.h"

#include <imgui_demo.cpp>
#include <iostream>

namespace FastSimDesign {
namespace SimMonitor {
ControllerWindow::ControllerWindow(Monitor* monitor) noexcept
  : Parent{monitor, "Controller Window", false}
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
  m_flags = window_flags;
  show();
}

void ControllerWindow::updateMenuBar(sf::Time const&)
{
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
      ImGui::MenuItem(
          "Show Entity Inspector",
          "Ctrl+I",
          &m_show_entity_inspector);
      ImGui::MenuItem("Show Debug Window", "Ctrl+D", &m_show_debug_window);
      if (ImGui::MenuItem(
              "Show Log Window",
              "Ctrl+L",
              m_monitor->getWindow<LogWindow>(Window::ID::LOG).isVisible()))
        m_monitor->getWindow<LogWindow>(Window::ID::LOG).switchVisibility();
      if (ImGui::MenuItem(
              "Show State Machine Window",
              "Ctrl+M",
              m_monitor
                  ->getWindow<StateMachineWindow>(Window::ID::STATE_MACHINE)
                  .isVisible()))
        m_monitor->getWindow<StateMachineWindow>(Window::ID::STATE_MACHINE)
            .switchVisibility();
      if (ImGui::MenuItem(
              "Show Scene Graph Window ",
              "Ctrl+G",
              m_monitor->getWindow<SceneGraphWindow>(Window::ID::SCENE_GRAPH)
                  .isVisible()))
        m_monitor->getWindow<SceneGraphWindow>(Window::ID::SCENE_GRAPH)
            .switchVisibility();
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("About"))
        m_monitor->openModalDialog(std::make_unique<AboutDialog>());
      ImGui::MenuItem("ImGui Demo", nullptr, &m_show_imgui_demo);
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }
}

void ControllerWindow::updateContentArea(sf::Time const&)
{
  if (m_show_imgui_demo)
    ImGui::ShowDemoWindow();
}
} // namespace SimMonitor
} // namespace FastSimDesign