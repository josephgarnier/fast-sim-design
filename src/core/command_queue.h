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
  class CommandQueue
  {
  public:
    explicit CommandQueue() = default; // Default constructor
    CommandQueue(CommandQueue const&) = default; // Copy constructor
    CommandQueue(CommandQueue&&) = default; // Move constructor
    CommandQueue& operator=(CommandQueue const&) = default; // Copy assignment operator
    CommandQueue& operator=(CommandQueue&&) = default; // Move assignment operator
    virtual ~CommandQueue() = default; // Destructor
    
    void push(Command const & command) noexcept;
    Command pop() noexcept;
    bool isEmpty() const noexcept;
    
  protected:
  private:

  private:
    std::queue<Command> m_queue;
  };
}
#endif