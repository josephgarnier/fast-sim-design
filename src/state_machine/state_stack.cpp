////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "state_stack.h"
#include <cassert>
#include <ranges>

namespace FastSimDesign {
  ////////////////////////////////////////////////////////////
  /// PendingChange::Methods
  ////////////////////////////////////////////////////////////
  StateStack::PendingChange::PendingChange(StateStack::Action action_, States::ID state_id_) noexcept
    : action{action_}
    , state_id{state_id_}
  {
  }

  ////////////////////////////////////////////////////////////
  /// StateStack::Methods
  ////////////////////////////////////////////////////////////
  StateStack::StateStack(State::Context context) noexcept
    : m_stack{}
    , m_pending_list{}
    , m_context{context}
    , m_factories{}
  {
  }
  void StateStack::handleEvent(sf::Event const& event) noexcept
  {
    // Iterate from top to bottom, stop as soon as handleEvent() returns false.
    for (auto const& state : std::ranges::reverse_view{m_stack})
    {
      if (!state->handleEvent(event))
        break;
    }
    applyPendingChanges();
  }
  void StateStack::update(sf::Time const& dt) noexcept
  {
    // Iterate from top to bottom, stop as soon as update() returns false.
    for (auto const& state : std::ranges::reverse_view{m_stack})
    {
      if (!state->update(dt))
        break;
    }
    applyPendingChanges();
  }

  void StateStack::draw() noexcept
  {
    // Draw all active states from bottom to top.
    for (auto const& state : m_stack)
      state->draw();
  }

  void StateStack::pushState(States::ID state_id) noexcept
  {
    m_pending_list.push_back(PendingChange(StateStack::Action::PUSH, state_id));
  }

  void StateStack::popState() noexcept
  {
    m_pending_list.push_back(PendingChange(StateStack::Action::POP));
  }

  void StateStack::clearStates() noexcept
  {
    m_pending_list.push_back(PendingChange(StateStack::Action::CLEAR));
  }

  bool StateStack::isEmpty() const noexcept
  {
    return m_stack.empty();
  }

  State::Ptr StateStack::createState(States::ID state_id) noexcept
  {
    auto found = m_factories.find(state_id);
    assert(found != m_factories.end());
    return found->second();
  }

  void StateStack::applyPendingChanges() noexcept
  {
    for (auto const& change : m_pending_list)
    {
      switch (change.action)
      {
        case StateStack::Action::PUSH:
          m_stack.push_back(createState(change.state_id));
          break;
        case StateStack::Action::POP:
          m_stack.pop_back();
          break;
        case StateStack::Action::CLEAR:
          m_stack.clear();
          break;
      }
    }
    m_pending_list.clear();
  }
}