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

#ifndef FAST_SIM_DESIGN_MENU_STATE_H
#define FAST_SIM_DESIGN_MENU_STATE_H

#include "state.h"
#include "../gui/container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>

namespace FastSimDesign {
  class MenuState final : public State
  {
  public:
    enum class OptionNames : uint16_t
    {
      PLAY,
      EXIT,
    };

  private:
    using Parent = State;

  public:
    explicit MenuState(StateStack* stack, Context context); // Default constructor
    virtual ~MenuState() = default; // Destructor

    virtual bool handleEvent(sf::Event const& event) override;
    virtual bool update(sf::Time const& dt) noexcept override;
    virtual void draw() override;

  private:
    sf::Sprite m_background_sprite;
    GUI::Container m_gui_container;
  };
}
#endif