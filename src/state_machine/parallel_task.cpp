////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "parallel_task.h"

#include <mutex>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
/// Statics
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Methods
////////////////////////////////////////////////////////////
ParallelTask::~ParallelTask()
{
  if (m_thread.joinable())
    m_thread.join();
}

void ParallelTask::execute() noexcept
{
  m_finished = false;
  m_elapsed_time.restart();
  m_thread = std::thread{&ParallelTask::runTask, this};
}

bool ParallelTask::isFinished() noexcept
{
  std::lock_guard<std::mutex> lock{m_mutex};
  return m_finished;
}

float ParallelTask::getCompletion() noexcept
{
  std::lock_guard<std::mutex> lock{m_mutex};

  // 100% at 10 seconds of elapsed time.
  return m_elapsed_time.getElapsedTime().asSeconds() / 10.f;
}

void ParallelTask::runTask() noexcept
{
  // Dummy task - stall 10 seconds.
  bool ended = false;
  while (!ended)
  {
    std::lock_guard<std::mutex> lock{m_mutex};
    if (m_elapsed_time.getElapsedTime().asSeconds() >= 10.f)
      ended = true;
  }

  {
    std::lock_guard<std::mutex> lock{m_mutex};
    m_finished = true;
  }
}

} // namespace FastSimDesign