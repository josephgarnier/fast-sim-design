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

#ifndef FAST_SIM_DESIGN_GAME_OVER_STATE_H
#define FAST_SIM_DESIGN_GAME_OVER_STATE_H

#include "state.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

namespace FastSimDesign {
  class GameOverState final : public State
  {
  private:
    using Parent = State;

  public:
    explicit GameOverState(StateStack* stack, Context context); // Default constructor
    GameOverState(GameOverState const&) = default; // Copy constructor
    GameOverState(GameOverState&&) = default; // Move constructor
    GameOverState& operator=(GameOverState const&) = default; // Copy assignment operator
    GameOverState& operator=(GameOverState&&) = default; // Move assignment operator
    virtual ~GameOverState() = default; // Destructor

    virtual bool handleEvent(sf::Event const& event) override;
    virtual bool update(sf::Time const& dt) noexcept override;
    virtual void draw() override;

  private:
    sf::Text m_game_over_text;
    sf::Time m_elapsed_time;
  };
}
#endif