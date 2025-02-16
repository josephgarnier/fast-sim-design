////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "game_state.h"
#include "../entity/player.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace FastSimDesign {
  GameState::GameState(StateStack* stack, Context context) noexcept
    : Parent{stack, std::move(context), "GAME"}
    , m_world{*context.window}
    , m_player{context.player}
  {
  }

  bool GameState::handleEvent(sf::Event const& event)
  {
    CommandQueue& commands = m_world.getCommandQueue();
    m_player->handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
      requestStackPush(States::ID::PAUSE);

    return true;
  }

  bool GameState::update(sf::Time const & dt)
  {
    m_world.update(dt);
    CommandQueue& commands = m_world.getCommandQueue();
    m_player->handleRealtimeInput(commands);
    return true;
  }

  void GameState::draw()
  {
    m_world.draw();
  }
}