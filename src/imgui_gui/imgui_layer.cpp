////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "imgui_layer.h"
#include "style_spectrum.h"
#include "imgui_layer_exception.h"

#include <imgui-SFML.h>
#include <imgui_internal.h>
#include <vector>

namespace FastSimDesign {

  ImGuiLayer::ImGuiLayer() noexcept
    : m_controller_window{this}
    , m_log_window{this}
    , m_state_machine_window{this}
    , m_open_modal_dialogs{}
  {
  }

  void ImGuiLayer::initImGui(Configuration const& config, sf::RenderWindow& window) const
  {
    if (!ImGui::SFML::Init(window))
      throw ImGuiLayerException{"ImGui-SFML failed to initialize"};

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

  void ImGuiLayer::setSpectrumStyle() const
  {
    ImGui::GetIO().Fonts->Clear();
    ImGui::Spectrum::StyleColorsSpectrum();
    ImGui::Spectrum::LoadFont(16.f);
    if (!ImGui::SFML::UpdateFontTexture())
      throw ImGuiLayerException{"ImGui-SFML failed to update font texture"};
  }

  void ImGuiLayer::handleEvent(sf::RenderWindow& window, sf::Event const& event) const
  {
    ImGui::SFML::ProcessEvent(window, event);
  }

  void ImGuiLayer::startImGuiUpdate(sf::RenderWindow& window, sf::Time const& dt)
  {
    ImGui::SFML::Update(window, dt);
  }

  void ImGuiLayer::updateImGui(sf::RenderWindow& window, sf::Time const& dt)
  {
    createDockSpace();
    m_controller_window.updateWindow(window, dt);
    m_log_window.updateWindow(window, dt);
    m_state_machine_window.updateWindow(window, dt);

    std::erase_if(m_open_modal_dialogs, [dt](std::unique_ptr<ModalDialog> const& modal) {
      modal->updateModal(dt);
      return !modal->isOpen();
    });
  }

  void ImGuiLayer::endImGuiUpdate()
  {
    ImGui::EndFrame();
  }

  void ImGuiLayer::createDockSpace() const
  {
    ImGuiID dockspace_id = configureDockSpace();
    configureDockNodes(dockspace_id);
  }

  ImGuiID ImGuiLayer::configureDockSpace() const
  {
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDecoration;

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 0.f});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0, 0, 0, 0}); // Transparent window background
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4{0, 0, 0, 0}); // Transparent dockspace background

    ImGui::Begin("ImGuiLayer", nullptr, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("ImGuiLayerDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2{0.f, 0.f}, dockspace_flags);
    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    return dockspace_id;
  }

  void ImGuiLayer::configureDockNodes(ImGuiID dockspace_id) const
  {
    static bool initialized = false;
    if (!initialized)
    {
      initialized = true;

      // Divide the space into two parts.
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
      [[maybe_unused]] ImGuiID dock_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);
      [[maybe_unused]] ImGuiID dock_right = dockspace_id;
      ImGui::DockBuilderFinish(dockspace_id);
    }
  }

  void ImGuiLayer::renderImGui(sf::RenderWindow& window)
  {
    ImGui::SFML::Render(window);
  }

  void ImGuiLayer::disposeImGui()
  {
    ImGui::SFML::Shutdown();
  }

  void ImGuiLayer::openModalDialog(std::unique_ptr<ModalDialog>&& modal_dialog)
  {
    m_open_modal_dialogs.push_back(std::move(modal_dialog));
  }

}