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

#include "../core/music_player.h"
#include "../entity/player.h"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace FastSimDesign {
GameState::GameState(StateStack* stack, Context context) noexcept
  : Parent{stack, context, "GAME"}
  , m_world{*context.monitor, *context.window, *context.fonts, *context.sounds}
  , m_player{context.player}
{
  m_player->setMissionStatus(Player::MissionStatus::MISSION_RUNNING);

  // Play menu theme.
  context.music->play(Music::ID::MISSION_THEME);
}

bool GameState::handleEvent(sf::Event const& event)
{
  // Game input handling.
  CommandQueue& commands = m_world.getCommandQueue();
  m_player->handleEvent(event, commands);

  // Escape pressed, trigger the pause screen.
  if (event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::Escape)
    requestStackPush(States::ID::PAUSE);

  return true;
}

bool GameState::update(sf::Time const& dt)
{
  m_world.update(dt);

  if (!m_world.hasAlivePlayer())
  {
    m_player->setMissionStatus(Player::MissionStatus::MISSION_FAILURE);
    requestStackPush(States::ID::GAME_OVER);
  }
  else if (m_world.hasPlayerReachedEnd())
  {
    m_player->setMissionStatus(Player::MissionStatus::MISSION_SUCCESS);
    requestStackPush(States::ID::GAME_OVER);
  }

  CommandQueue& commands = m_world.getCommandQueue();
  m_player->handleRealtimeInput(commands);

  return true;
}

void GameState::draw()
{
  m_world.draw();
}
} // namespace FastSimDesign