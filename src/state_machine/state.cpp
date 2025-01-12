////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "state.h"
#include "state_stack.h"

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// Context::Methods
  ////////////////////////////////////////////////////////////
  State::Context::Context(sf::RenderWindow* window_, TextureHolder* textures_, FontHolder* fonts_, Player* player_) noexcept
    : window{window_}
    , textures{textures_}
    , fonts{fonts_}
    , player{player_}
  {
  }

  ////////////////////////////////////////////////////////////
  /// State::Methods
  ////////////////////////////////////////////////////////////
  State::State(StateStack* stack, Context context) noexcept
    : m_stack{stack}
    , m_context{context}
  {
  }

  void State::requestStackPush(States::ID state_id) noexcept
  {
    m_stack->pushState(state_id);
  }

  void State::requestStackPop() noexcept
  {
    m_stack->popState();
  }

  void State::requestStateClear() noexcept
  {
    m_stack->clearStates();
  }

  State::Context State::getContext() const noexcept
  {
    return m_context;
  }
}