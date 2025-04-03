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

#ifndef FAST_SIM_DESIGN_COMMAND_QUEUE_H
#define FAST_SIM_DESIGN_COMMAND_QUEUE_H

#include "command.h"

#include <queue>

namespace FastSimDesign {
class CommandQueue final
{
public:
  explicit CommandQueue() = default;
  CommandQueue(CommandQueue const&) = default;
  CommandQueue(CommandQueue&&) = default;
  CommandQueue& operator=(CommandQueue const&) = default;
  CommandQueue& operator=(CommandQueue&&) = default;
  virtual ~CommandQueue() = default;

  void push(Command const& command) noexcept;
  Command pop() noexcept;
  bool isEmpty() const noexcept;

private:
  std::queue<Command> m_queue{};
};
} // namespace FastSimDesign
#endif