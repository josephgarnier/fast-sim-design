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

#ifndef FAST_SIM_DESIGN_PARALLEL_TASK_H
#define FAST_SIM_DESIGN_PARALLEL_TASK_H

#include <SFML/System/Clock.hpp>
#include <SFML/System/Mutex.hpp>
// #include <SFML/System/Thread.hpp>

#include <mutex>
#include <thread>

namespace FastSimDesign {
  class ParallelTask
  {
  public:
    explicit ParallelTask() noexcept; // Default constructor
    virtual ~ParallelTask(); // Destructor

    void execute() noexcept;
    bool isFinished() noexcept;
    float getCompletion() noexcept;

  private:
    void runTask() noexcept;

  private:
    std::thread m_thread;
    bool m_finished;
    sf::Clock m_elapsed_time;
    std::mutex m_mutex;
  };
}
#endif