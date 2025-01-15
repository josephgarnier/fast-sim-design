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

#ifndef FAST_SIM_DESIGN_COMMAND_H
#define FAST_SIM_DESIGN_COMMAND_H

#include <SFML/System/Time.hpp>
#include <functional>
#include <cassert>

namespace FastSimDesign {
  class SceneNode;
  struct Command
  {
    explicit Command() noexcept; // Default constructor
    Command(Command const&) = default; // Copy constructor
    Command(Command&&) = default; // Move constructor
    Command& operator=(Command const&) = default; // Copy assignment operator
    Command& operator=(Command&&) = default; // Move assignment operator
    virtual ~Command() = default; // Destructor

    std::function<void(SceneNode&, sf::Time)> action;
    unsigned int category;
  };

  template<typename GameObject, typename Function>
  std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
  {
    return [=](SceneNode& node, sf::Time const& dt) {
      // Check if cast is safe
      assert(dynamic_cast<GameObject*>(&node) != nullptr);

      // Downcast node and invoke function on it
      fn(static_cast<GameObject&>(node), dt);
    };
  }
}
#endif