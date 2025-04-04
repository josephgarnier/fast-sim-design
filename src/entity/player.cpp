////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "player.h"

#include "aircraft.h"
#include "category.h"

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////
bool Player::isRealtimeAction(Player::Action action) noexcept
{
  switch (action)
  {
    case Player::Action::MOVE_LEFT:
    case Player::Action::MOVE_RIGHT:
    case Player::Action::MOVE_UP:
    case Player::Action::MOVE_DOWN:
    case Player::Action::FIRE:
      return true;
    default:
      return false;
  }
}

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
Player::Player()
  : m_current_mission_status{Player::MissionStatus::MISSION_RUNNING}
{
  // Set initial key bindings.
  m_key_binding[sf::Keyboard::Key::Left] = Player::Action::MOVE_LEFT;
  m_key_binding[sf::Keyboard::Key::Right] = Player::Action::MOVE_RIGHT;
  m_key_binding[sf::Keyboard::Key::Up] = Player::Action::MOVE_UP;
  m_key_binding[sf::Keyboard::Key::Down] = Player::Action::MOVE_DOWN;
  m_key_binding[sf::Keyboard::Key::Space] = Player::Action::FIRE;
  m_key_binding[sf::Keyboard::Key::M] = Player::Action::LAUNCH_MISSILE;

  // Set initial action bindings.
  initializeActions();

  for (auto& pair : m_action_binding)
    pair.second.category =
        BitFlags<Category::Type>{Category::Type::PLAYER_AIRCRAFT};
}

void Player::handleEvent(sf::Event const& event, CommandQueue& commands)
{
  if (event.type == sf::Event::KeyPressed)
  {
    // Check if pressed key appears in key binding, trigger command if so.
    auto found = m_key_binding.find(event.key.code);
    if (found != m_key_binding.end() && !isRealtimeAction(found->second))
      commands.push(m_action_binding[found->second]);
  }
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
  // Traverse all assigned keys and check if they are pressed.
  for (auto const& pair : m_key_binding)
  {
    // If key is pressed, lookup action and trigger corresponding command.
    if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
      commands.push(m_action_binding[pair.second]);
  }
}

void Player::assignKey(Player::Action action, sf::Keyboard::Key key)
{
  // Remove all keys that already map to action
  for (auto it = m_key_binding.begin(); it != m_key_binding.end();)
  {
    if (it->second == action)
      m_key_binding.erase(it++);
    else
      ++it;
  }

  // Insert new binding
  m_key_binding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Player::Action action) const noexcept
{
  for (auto const& pair : m_key_binding)
  {
    if (pair.second == action)
      return pair.first;
  }
  return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(Player::MissionStatus status) noexcept
{
  m_current_mission_status = status;
}

Player::MissionStatus Player::getMissionStatus() const noexcept
{
  return m_current_mission_status;
}

void Player::initializeActions()
{
  float const player_speed = 80.f;
  m_action_binding[Player::Action::MOVE_LEFT].name = "MoveLeft";
  m_action_binding[Player::Action::MOVE_LEFT].action =
      derivedAction<Aircraft>(AircraftMover{-player_speed, 0.f});
  m_action_binding[Player::Action::MOVE_RIGHT].name = "MoveRight";
  m_action_binding[Player::Action::MOVE_RIGHT].action =
      derivedAction<Aircraft>(AircraftMover{+player_speed, 0.f});
  m_action_binding[Player::Action::MOVE_UP].name = "MoveUp";
  m_action_binding[Player::Action::MOVE_UP].action =
      derivedAction<Aircraft>(AircraftMover{0.f, -player_speed});
  m_action_binding[Player::Action::MOVE_DOWN].name = "MoveDown";
  m_action_binding[Player::Action::MOVE_DOWN].action =
      derivedAction<Aircraft>(AircraftMover{0.f, +player_speed});
  m_action_binding[Player::Action::FIRE].name = "Fire";
  m_action_binding[Player::Action::FIRE].action =
      derivedAction<Aircraft>([](Aircraft& aircraft, sf::Time) {
        aircraft.fire();
      });
  m_action_binding[Player::Action::LAUNCH_MISSILE].name = "LaunchMissile";
  m_action_binding[Player::Action::LAUNCH_MISSILE].action =
      derivedAction<Aircraft>([](Aircraft& aircraft, sf::Time) {
        aircraft.launchMissile();
      });
}
} // namespace FastSimDesign