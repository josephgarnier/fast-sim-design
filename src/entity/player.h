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

#include "../core/command_queue.h"
#include "aircraft.h"

#include <SFML/Window/Event.hpp>

#include <cstdint>
#include <map>

namespace FastSimDesign {
class Player
{
public:
  enum class Action : uint16_t
  {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    FIRE,
    LAUNCH_MISSILE,
    ACTION_COUNT
  };
  enum class MissionStatus : uint16_t
  {
    MISSION_RUNNING,
    MISSION_SUCCESS,
    MISSION_FAILURE
  };

public:
  static bool isRealtimeAction(Player::Action action) noexcept;

public:
  explicit Player();
  Player(Player const&) = default;
  Player(Player&&) = default;
  Player& operator=(Player const&) = default;
  Player& operator=(Player&&) = default;
  virtual ~Player() = default;

  void handleEvent(sf::Event const& event, CommandQueue& commands);
  void handleRealtimeInput(CommandQueue& commands);

  void assignKey(Player::Action action, sf::Keyboard::Key key);
  sf::Keyboard::Key getAssignedKey(Player::Action action) const noexcept;

  void setMissionStatus(Player::MissionStatus status) noexcept;
  Player::MissionStatus getMissionStatus() const noexcept;

protected:
private:
  void initializeActions();

public:
protected:
private:
  std::map<sf::Keyboard::Key, Player::Action> m_key_binding{};
  std::map<Player::Action, Command> m_action_binding{};
  Player::MissionStatus m_current_mission_status{};
};

struct AircraftMover
{
  explicit AircraftMover(float vx, float vy) noexcept
    : m_velocity(vx, vy)
  {
  }

  void operator()(Aircraft& aircraft, sf::Time) const noexcept
  {
    aircraft.accelerate(m_velocity);
  }

  sf::Vector2f m_velocity;
};
} // namespace FastSimDesign
#endif
