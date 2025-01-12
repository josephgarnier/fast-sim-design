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
#include "category.h"
#include "../utils/generic_utility.h"

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
        return true;
      default:
        return false;
    }
  }

  ////////////////////////////////////////////////////////////
  /// Methods
  ////////////////////////////////////////////////////////////
  Player::Player() noexcept
    : m_key_binding{}
    , m_action_binding{}
  {
    // Set initial key bindings.
    m_key_binding[sf::Keyboard::Key::Left] = Player::Action::MOVE_LEFT;
    m_key_binding[sf::Keyboard::Key::Right] = Player::Action::MOVE_RIGHT;
    m_key_binding[sf::Keyboard::Key::Up] = Player::Action::MOVE_UP;
    m_key_binding[sf::Keyboard::Key::Down] = Player::Action::MOVE_DOWN;

    // Set initial action bindings.
    initializeActions();

    for (auto& pair : m_action_binding)
      pair.second.category = toUnderlyingType(Category::Type::PLAYER_AIRCRAFT);
  }

  void Player::handleEvent(sf::Event const& event, CommandQueue& commands) noexcept
  {
    if (event.type == sf::Event::KeyPressed)
    {
      // Check if pressed key appears in key binding, trigger command if so.
      auto found = m_key_binding.find(event.key.code);
      if (found != m_key_binding.end() && !isRealtimeAction(found->second))
        commands.push(m_action_binding[found->second]);
    }
  }

  void Player::handleRealtimeInput(CommandQueue& commands) noexcept
  {
    // Traverse all assigned keys and check if they are pressed.
    for (auto const& pair : m_key_binding)
    {
      // If key is pressed, lookup action and trigger corresponding command.
      if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
        commands.push(m_action_binding[pair.second]);
    }
  }

  void Player::assignKey(Player::Action action, sf::Keyboard::Key key) noexcept
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

  void Player::initializeActions() noexcept
  {
    float const player_speed = 10.f;
    m_action_binding[Player::Action::MOVE_LEFT].action = derivedAction<Aircraft>(AircraftMover(-player_speed, 0.f));
    m_action_binding[Player::Action::MOVE_RIGHT].action = derivedAction<Aircraft>(AircraftMover(+player_speed, 0.f));
    m_action_binding[Player::Action::MOVE_UP].action = derivedAction<Aircraft>(AircraftMover(0.f, -player_speed));
    m_action_binding[Player::Action::MOVE_DOWN].action = derivedAction<Aircraft>(AircraftMover(0.f, +player_speed));
  }
}