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
StateStack::PendingChange::PendingChange(
    StateStack::Action action_, States::ID state_id_) noexcept
  : action{action_}
  , state_id{state_id_}
{
}

////////////////////////////////////////////////////////////
/// StateStack::Methods
////////////////////////////////////////////////////////////
StateStack::StateStack(State::Context context) noexcept
  : m_context{std::move(context)}
{
}
void StateStack::handleEvent(sf::Event const& event)
{
  // Iterate from top to bottom, stop as soon as handleEvent() returns false.
  for (auto const& state : std::ranges::reverse_view{m_stack})
  {
    if (!state->handleEvent(event))
      break;
  }
  applyPendingChanges();
}
void StateStack::update(sf::Time const& dt)
{
  // Iterate from top to bottom, stop as soon as update() returns false.
  for (auto const& state : std::ranges::reverse_view{m_stack})
  {
    if (!state->update(dt))
      break;
  }
  applyPendingChanges();
}

void StateStack::draw()
{
  // Draw all active states from bottom to top.
  for (auto const& state : m_stack)
    state->draw();
}

void StateStack::monitorState(
    SimMonitor::Monitor& monitor,
    SimMonitor::Frame::StateMachine& frame_states_stack) const
{
  frame_states_stack.states.clear();
  frame_states_stack.states.reserve(m_stack.size());
  for (auto const& state : std::views::reverse(m_stack))
  {
    SimMonitor::Frame::StateMachineState state_machine_state{};
    state->monitorState(monitor, state_machine_state);
    frame_states_stack.states.push_back(std::move(state_machine_state));
  }
}

void StateStack::pushState(States::ID state_id)
{
  m_pending_list.push_back(PendingChange(StateStack::Action::PUSH, state_id));
}

void StateStack::popState()
{
  m_pending_list.push_back(PendingChange(StateStack::Action::POP));
}

void StateStack::clearStates()
{
  m_pending_list.push_back(PendingChange(StateStack::Action::CLEAR));
}

bool StateStack::isEmpty() const noexcept
{
  return m_stack.empty();
}

State::Ptr StateStack::createState(States::ID state_id)
{
  auto found = m_factories.find(state_id);
  assert(found != m_factories.end());
  return found->second();
}

void StateStack::applyPendingChanges()
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
} // namespace FastSimDesign