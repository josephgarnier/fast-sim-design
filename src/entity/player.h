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

#ifndef FAST_SIM_DESIGN_PLAYER_H
#define FAST_SIM_DESIGN_PLAYER_H

#include "aircraft.h"
#include "../core/command_queue.h"
#include <SFML/Window/Event.hpp>
#include <map>

namespace FastSimDesign {
  class Player
  {
  public:
    enum class Action : int
    {
      MOVE_LEFT,
      MOVE_RIGHT,
      MOVE_UP,
      MOVE_DOWN,
      ACTION_COUNT
    };

  public:
    static bool isRealtimeAction(Player::Action action) noexcept;

  public:
    explicit Player() noexcept; // Default constructor
    Player(Player const&) = default; // Copy constructor
    Player(Player&&) = default; // Move constructor
    Player& operator=(Player const&) = default; // Copy assignment operator
    Player& operator=(Player&&) = default; // Move assignment operator
    virtual ~Player() = default; // Destructor

    void handleEvent(sf::Event const& event, CommandQueue& commands) noexcept;
    void handleRealtimeInput(CommandQueue& commands) noexcept;

    void assignKey(Player::Action action, sf::Keyboard::Key key) noexcept;
    sf::Keyboard::Key getAssignedKey(Player::Action action) const noexcept;

  protected:
  private:
    void initializeActions() noexcept;

  public:
  protected:
  private:
    std::map<sf::Keyboard::Key, Player::Action> m_key_binding;
    std::map<Player::Action, Command> m_action_binding;
  };

  struct AircraftMover
  {
    explicit AircraftMover(float vx, float vy) noexcept
      : m_velocity(vx, vy)
    {
    }

    void operator()(Aircraft& aircraft, sf::Time) const
    {
      aircraft.accelerate(m_velocity);
    }

    sf::Vector2f m_velocity;
  };
}
#endif
