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

#ifndef FAST_SIM_DESIGN_PAUSE_STATE_H
#define FAST_SIM_DESIGN_PAUSE_STATE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "state.h"

namespace FastSimDesign {
  class PauseState : public State
  {
  public:
  private:
    using Parent = State;

  public:
    explicit PauseState(StateStack* stack, Context context); // Default constructor
    virtual ~PauseState() = default; // Destructor

    virtual bool handleEvent(sf::Event const& event) override;
    virtual bool update(sf::Time const & dt) noexcept override;
    virtual void draw() override;

  public:
  protected:
  private:
    sf::Sprite m_background_sprite;
    sf::Text m_paused_text;
    sf::Text m_instruction_text;
  };
}
#endif