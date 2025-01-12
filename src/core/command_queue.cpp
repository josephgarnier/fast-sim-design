////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "command_queue.h"

namespace FastSimDesign {
  void CommandQueue::push(Command const& command) noexcept
  {
    m_queue.push(command);
  }

  Command CommandQueue::pop() noexcept
  {
    Command command = m_queue.front();
    m_queue.pop();
    return command;
  }

  bool CommandQueue::isEmpty() const noexcept
  {
    return m_queue.empty();
  }
}