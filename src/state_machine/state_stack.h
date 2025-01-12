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

#include "state_identifiers.h"
#include "state.h"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <memory>
#include <vector>
#include <functional>
#include <map>

namespace FastSimDesign {
  class StateStack : private sf::NonCopyable
  {
  public:
    enum class Action
    {
      PUSH,
      POP,
      CLEAR,
    };

  private:
    struct PendingChange
    {
      explicit PendingChange(StateStack::Action action_, States::ID state_id_ = States::ID::NONE) noexcept; // Default constructor
      PendingChange(PendingChange const&) = default; // Copy constructor
      PendingChange(PendingChange&&) = default; // Move constructor
      PendingChange& operator=(PendingChange const&) = default; // Copy assignment operator
      PendingChange& operator=(PendingChange&&) = default; // Move assignment operator
      virtual ~PendingChange() = default; // Destructor

      Action action;
      States::ID state_id;
    };

  public:
    explicit StateStack(State::Context context) noexcept; // Default constructor
    virtual ~StateStack() = default; // Destructor

    template<typename T>
    void registerState(States::ID state_id) noexcept
    {
      m_factories[state_id] = [this]() {
        return std::make_unique<T>(this, m_context);
      };
    };

    void handleEvent(sf::Event const& event) noexcept;
    void update(sf::Time const & dt) noexcept;
    void draw() noexcept;

    void pushState(States::ID state_id) noexcept;
    void popState() noexcept;
    void clearStates() noexcept;

    bool isEmpty() const noexcept;

  private:
    State::Ptr createState(States::ID state_id) noexcept;
    void applyPendingChanges() noexcept;

  private:
    std::vector<State::Ptr> m_stack;
    std::vector<PendingChange> m_pending_list;

    State::Context m_context;
    std::map<States::ID, std::function<State::Ptr()>> m_factories;
  };
}
#endif