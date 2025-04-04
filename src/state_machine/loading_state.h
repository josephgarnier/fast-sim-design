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

#ifndef FAST_SIM_DESIGN_LOADING_STATE_H
#define FAST_SIM_DESIGN_LOADING_STATE_H

#include "parallel_task.h"
#include "state.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace FastSimDesign {
class LoadingState final : public State
{
private:
  using Parent = State;

public:
  explicit LoadingState(StateStack* stack, Context context);
  virtual ~LoadingState() = default;

  virtual bool handleEvent(sf::Event const& event) override;
  virtual bool update(sf::Time const& dt) noexcept override;
  virtual void draw() override;

private:
  void setCompletion(float percent) noexcept;

private:
  sf::Text m_loading_text{};
  sf::RectangleShape m_progress_bar_background{};
  sf::RectangleShape m_progress_bar{};

  ParallelTask m_loading_task{};
};
} // namespace FastSimDesign
#endif