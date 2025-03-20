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


#ifndef FAST_SIM_DESIGN_SETTINGS_STATE_H
#define FAST_SIM_DESIGN_SETTINGS_STATE_H

#include "state.h"
#include "../entity/player.h"
#include "../gui/label.h"
#include "../gui/button.h"
#include "../gui/container.h"
#include "../utils/generic_utility.h"

#include <array>

namespace FastSimDesign {
  class SettingsState final : public State
  {
  public:
  private:
    using Parent = State;

  public:
    explicit SettingsState(StateStack* stack, Context context); // Default constructor
    virtual ~SettingsState() = default; // Destructor

    virtual bool handleEvent(sf::Event const& event) override;
    virtual bool update(sf::Time const& dt) override;
    virtual void draw() override;

  private:
    void updateLabels() noexcept;
    void addButtonLabel(Player::Action action, float y, std::string text, Context context) noexcept;

  private:
    sf::Sprite m_background_sprite;
    GUI::Container m_gui_container;
    std::array<GUI::Button::Ptr, toUnderlyingType(Player::Action::ACTION_COUNT)> m_binding_buttons;
    std::array<GUI::Label::Ptr, toUnderlyingType(Player::Action::ACTION_COUNT)> m_binding_labels;
  };
}
#endif