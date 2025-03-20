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

#include "state.h"
#include "../core/world.h"

namespace FastSimDesign {
  class GameState final : public State
  {
  private:
    using Parent = State;

  public:
    explicit GameState(StateStack* stack, Context context) noexcept; // Default constructor
    virtual ~GameState() = default; // Destructor

    virtual bool handleEvent(sf::Event const& event) override;
    virtual bool update(sf::Time const& dt) override;
    virtual void draw() override;

  public:
  protected:
  private:
    World m_world;
    Player* m_player;
  };
}
#endif