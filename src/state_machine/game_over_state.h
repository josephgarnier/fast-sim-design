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
  explicit GameOverState(StateStack* stack, Context context);
  GameOverState(GameOverState const&) = default;
  GameOverState(GameOverState&&) = default;
  GameOverState& operator=(GameOverState const&) = default;
  GameOverState& operator=(GameOverState&&) = default;
  virtual ~GameOverState() = default;

  virtual bool handleEvent(sf::Event const& event) override;
  virtual bool update(sf::Time const& dt) noexcept override;
  virtual void draw() override;

private:
  sf::Text m_game_over_text{};
  sf::Time m_elapsed_time{sf::Time::Zero};
};
} // namespace FastSimDesign
#endif