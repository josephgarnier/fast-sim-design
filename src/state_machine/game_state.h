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

#ifndef FAST_SIM_DESIGN_GAME_STATE_H
#define FAST_SIM_DESIGN_GAME_STATE_H

#include "../core/world.h"
#include "state.h"

namespace FastSimDesign {
class GameState final : public State
{
private:
  using Parent = State;

public:
  explicit GameState(StateStack* stack, Context context) noexcept;
  virtual ~GameState() = default;

  virtual bool handleEvent(sf::Event const& event) override;
  virtual bool update(sf::Time const& dt) override;
  virtual void draw() override;

private:
  World m_world;
  Player* m_player{nullptr};
};
} // namespace FastSimDesign
#endif