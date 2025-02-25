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
  State::State(StateStack* stack, Context context, std::string name) noexcept
    : m_stack{stack}
    , m_context{std::move(context)}
    , m_name{std::move(name)}
  {
  }

  void State::requestStackPush(States::ID state_id)
  {
    m_stack->pushState(state_id);
  }

  void State::requestStackPop()
  {
    m_stack->popState();
  }

  void State::requestStateClear()
  {
    m_stack->clearStates();
  }

  State::Context State::getContextCopy() const noexcept
  {
    return m_context;
  }
  
  State::Context& State::getContext() noexcept
  {
    return m_context;
  }
}