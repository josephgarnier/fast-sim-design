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

#ifndef FAST_SIM_DESIGN_TITLE_STATE_H
#define FAST_SIM_DESIGN_TITLE_STATE_H

#include "state.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

namespace FastSimDesign {
class TitleState final : public State
{
private:
  using Parent = State;

public:
  explicit TitleState(StateStack* stack, Context context);
  virtual ~TitleState() = default;

  virtual bool handleEvent(sf::Event const& event) override;
  virtual bool update(sf::Time const& dt) noexcept override;
  virtual void draw() override;

public:
protected:
private:
  sf::Sprite m_background_sprite{};
  sf::Text m_text{};
  bool m_show_text{true};
  sf::Time m_text_effect_time{sf::Time::Zero};
};
} // namespace FastSimDesign
#endif