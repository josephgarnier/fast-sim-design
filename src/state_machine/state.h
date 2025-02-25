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

#include "state_identifiers.h"
#include "../core/resource_identifiers.h"

#include <memory>

namespace sf {
  class RenderWindow;
  class Time;
  class Event;
}

namespace FastSimDesign {

  class StateStack;
  class Player;
  class State
  {
  public:
    struct Context
    {
      explicit Context(sf::RenderWindow* window_, TextureHolder* textures_, FontHolder* fonts_, Player* player_) noexcept; // Default constructor
      Context(Context const&) = default; // Copy constructor
      Context(Context&&) = default; // Move constructor
      Context& operator=(Context const&) = default; // Copy assignment operator
      Context& operator=(Context&&) = default; // Move assignment operator
      virtual ~Context() = default; // Destructor

      sf::RenderWindow* window;
      TextureHolder* textures;
      FontHolder* fonts;
      Player* player;
    };

  public:
    using Ptr = std::unique_ptr<State>;

  public:
    explicit State(StateStack* stack, Context context, std::string name) noexcept; // Default constructor
    State(State const&) = default; // Copy constructor
    State(State&&) = default; // Move constructor
    State& operator=(State const&) = default; // Copy assignment operator
    State& operator=(State&&) = default; // Move assignment operator
    virtual ~State() = default; // Destructor

    std::string const& getName() const noexcept { return m_name; }

    virtual bool handleEvent(sf::Event const& event) = 0;
    virtual bool update(sf::Time const& dt) = 0;
    virtual void draw() = 0;

  protected:
    void requestStackPush(States::ID state_id);
    void requestStackPop();
    void requestStateClear();

    State::Context getContextCopy() const noexcept;
    State::Context& getContext() noexcept;

  private:
    StateStack* m_stack;
    Context m_context;
    std::string m_name;
  };
}
#endif