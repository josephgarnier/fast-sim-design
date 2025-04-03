////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "monitor.h"

#include "style_spectrum.h"
#include "window/controller_window.h"
#include "window/log_window.h"
#include "window/scene_graph_window.h"
#include "window/state_machine_window.h"

#include <imgui-SFML.h>
#include <imgui_internal.h>

namespace FastSimDesign {

namespace SimMonitor {
////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
Monitor::Monitor() noexcept
{
  createWindow<ControllerWindow>(Window::ID::CONTROLLER);
  // createWindow<DebugWindow>(Window::ID::DEBUG);
  createWindow<LogWindow>(Window::ID::LOG);
  createWindow<StateMachineWindow>(Window::ID::STATE_MACHINE);
  createWindow<SceneGraphWindow>(Window::ID::SCENE_GRAPH);
}

void Monitor::initImGui(
    Configuration const& config, sf::RenderWindow& window) const
{
  if (!ImGui::SFML::Init(window))
    throw MonitorException{"ImGui-SFML failed to initialize"};

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  if (config.use_custom_style)
    setSpectrumStyle();
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
}

void Monitor::setSpectrumStyle() const
{
  ImGui::GetIO().Fonts->Clear();
  ImGui::Spectrum::StyleColorsSpectrum();
  ImGui::Spectrum::LoadFont(16.f);
  if (!ImGui::SFML::UpdateFontTexture())
    throw MonitorException{"ImGui-SFML failed to update font texture"};
}

void Monitor::handleEvent(
    sf::RenderWindow& window, sf::Event const& event) const
{
  ImGui::SFML::ProcessEvent(window, event);
}

void Monitor::updateImGui(sf::RenderWindow& window, sf::Time const& dt)
{
  ImGui::SFML::Update(window, dt);

  createDockSpace();
  for (auto const& [pId, pWindow] : m_windows)
    pWindow->update(dt);

  std::erase_if(
      m_open_modal_dialogs,
      [dt](std::unique_ptr<ModalDialog> const& modal) {
        modal->updateModal(dt);
        return !modal->isOpen();
      });
}

void Monitor::createDockSpace() const
{
  ImGuiID dockspace_id = configureDockSpace();
  configureDockNodes(dockspace_id);
}

ImGuiID Monitor::configureDockSpace() const
{
  static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_NoDecoration;

  static ImGuiDockNodeFlags dockspace_flags =
      ImGuiDockNodeFlags_PassthruCentralNode;

  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 0.f});
  ImGui::PushStyleColor(
      ImGuiCol_WindowBg,
      ImVec4{0, 0, 0, 0}); // Transparent window background
  ImGui::PushStyleColor(
      ImGuiCol_DockingEmptyBg,
      ImVec4{0, 0, 0, 0}); // Transparent dockspace background

  ImGui::Begin("Monitor", nullptr, window_flags);
  ImGuiID dockspace_id = ImGui::GetID("MonitorDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2{0.f, 0.f}, dockspace_flags);
  ImGui::End();
  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor(2);

  return dockspace_id;
}

void Monitor::configureDockNodes(ImGuiID dockspace_id) const
{
  static bool initialized = false;
  if (!initialized)
  {
    initialized = true;

    // Divide the space into two parts.
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    [[maybe_unused]] ImGuiID dock_left = ImGui::DockBuilderSplitNode(
        dockspace_id,
        ImGuiDir_Left,
        0.5f,
        nullptr,
        &dockspace_id);
    [[maybe_unused]] ImGuiID dock_right = dockspace_id;
    ImGui::DockBuilderFinish(dockspace_id);
  }
}

void Monitor::renderImGui(sf::RenderWindow& window)
{
  ImGui::SFML::Render(window);
}

void Monitor::disposeImGui()
{
  ImGui::SFML::Shutdown();
}

void Monitor::openModalDialog(std::unique_ptr<ModalDialog>&& modal_dialog)
{
  m_open_modal_dialogs.push_back(std::move(modal_dialog));
}
} // namespace SimMonitor

} // namespace FastSimDesign