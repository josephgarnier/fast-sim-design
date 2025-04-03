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

#ifndef FAST_SIM_DESIGN_STATE_H
#define FAST_SIM_DESIGN_STATE_H

#include "../core/resource_identifiers.h"
#include "../monitor/monitorable.h"
#include "state_identifiers.h"

#include <memory>

namespace sf {
class RenderWindow;
class Time;
class Event;
} // namespace sf

namespace FastSimDesign {
class StateStack;
class Player;
class State : public SimMonitor::Monitorable
{
public:
  struct Context
  {
    explicit Context(
        SimMonitor::Monitor* monitor_,
        sf::RenderWindow* window_,
        TextureHolder* textures_,
        FontHolder* fonts_,
        Player* player_) noexcept;
    Context(Context const&) = default;
    Context(Context&&) = default;
    Context& operator=(Context const&) = default;
    Context& operator=(Context&&) = default;
    virtual ~Context() = default;

    SimMonitor::Monitor* monitor;
    sf::RenderWindow* window;
    TextureHolder* textures;
    FontHolder* fonts;
    Player* player;
  };

public:
  using Ptr = std::unique_ptr<State>;
  using Monitorable::monitorState;

private:
  using Parent = SimMonitor::Monitorable;

public:
  explicit State(StateStack* stack, Context context, std::string name) noexcept;
  State(State const&) = default;
  State(State&&) = default;
  State& operator=(State const&) = default;
  State& operator=(State&&) = default;
  virtual ~State() = default;

  virtual bool handleEvent(sf::Event const& event) = 0;
  virtual bool update(sf::Time const& dt) = 0;
  virtual void draw() = 0;

  virtual void monitorState(
      SimMonitor::Monitor& monitor,
      SimMonitor::Frame::StateMachineState& frame_object) const override final;

protected:
  void requestStackPush(States::ID state_id);
  void requestStackPop();
  void requestStateClear();

  State::Context getContextCopy() const noexcept;
  State::Context& getContext() noexcept;

private:
  StateStack* m_stack{nullptr};
  Context m_context;
  std::string m_name{};
};
} // namespace FastSimDesign
#endif