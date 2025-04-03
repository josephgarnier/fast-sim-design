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

#ifndef FAST_SIM_DESIGN_STATE_STACK_H
#define FAST_SIM_DESIGN_STATE_STACK_H

#include "../monitor/monitorable.h"
#include "state.h"
#include "state_identifiers.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace FastSimDesign {
class StateStack final
  : private sf::NonCopyable
  , public SimMonitor::Monitorable
{
public:
  enum class Action : uint16_t
  {
    PUSH,
    POP,
    CLEAR,
  };

private:
  struct PendingChange
  {
    explicit PendingChange(
        StateStack::Action action_,
        States::ID state_id_ = States::ID::NONE) noexcept;
    PendingChange(PendingChange const&) = default;
    PendingChange(PendingChange&&) = default;
    PendingChange& operator=(PendingChange const&) = default;
    PendingChange& operator=(PendingChange&&) = default;
    virtual ~PendingChange() = default;

    Action action;
    States::ID state_id;
  };

public:
  using Monitorable::monitorState;

private:
  using Parent = SimMonitor::Monitorable;

public:
  explicit StateStack(State::Context context) noexcept;
  virtual ~StateStack() = default;

  template<typename T>
  void registerState(States::ID state_id)
  {
    static_assert(std::is_base_of_v<State, T>, "T must derive from State");
    m_factories[state_id] = [this]() {
      return std::make_unique<T>(this, m_context);
    };
  };

  void handleEvent(sf::Event const& event);
  void update(sf::Time const& dt);
  void draw();

  virtual void monitorState(
      SimMonitor::Monitor& monitor,
      SimMonitor::Frame::StateMachine& frame_states_stack) const override final;

  void pushState(States::ID state_id);
  void popState();
  void clearStates();

  bool isEmpty() const noexcept;

private:
  State::Ptr createState(States::ID state_id);
  void applyPendingChanges();

private:
  std::vector<State::Ptr> m_stack{};
  std::vector<PendingChange> m_pending_list{};

  State::Context m_context;
  std::map<States::ID, std::function<State::Ptr()>> m_factories{};
};
} // namespace FastSimDesign
#endif