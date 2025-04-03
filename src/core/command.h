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

#include "../entity/category.h"

#include <SFML/System/Time.hpp>

#include <cassert>
#include <functional>
#include <string>

namespace FastSimDesign {
class SceneNode;
struct Command
{
  explicit Command() = default;
  Command(Command const&) = default;
  Command(Command&&) = default;
  Command& operator=(Command const&) = default;
  Command& operator=(Command&&) = default;
  virtual ~Command() = default;

  std::string name{""};
  BitFlags<Category::Type> category{Category::Type::NONE};
  std::function<void(SceneNode&, sf::Time)> action{};
};

template<typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function action)
{
  return [=](SceneNode& node, sf::Time const& dt) {
    // Check if cast is safe.
    assert(dynamic_cast<GameObject*>(&node) != nullptr);

    // Downcast node and invoke function on it.
    action(static_cast<GameObject&>(node), dt);
  };
}
} // namespace FastSimDesign
#endif