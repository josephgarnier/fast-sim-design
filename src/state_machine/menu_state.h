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
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

namespace FastSimDesign {
  class MenuState : public State
  {
  public:
    enum class OptionNames : int
    {
      PLAY,
      EXIT,
    };

  private:
    using Parent = State;

  public:
    explicit MenuState(StateStack* stack, Context context) noexcept; // Default constructor
    virtual ~MenuState() = default; // Destructor

    virtual bool handleEvent(sf::Event const& event) noexcept override;
    virtual bool update(sf::Time const & dt) noexcept override;
    virtual void draw() noexcept override;

    void updateOptionText() noexcept;

  public:
  protected:
  private:
    sf::Sprite m_background_sprite;
    std::vector<sf::Text> m_options;
    std::size_t m_option_index;
  };
}
#endif