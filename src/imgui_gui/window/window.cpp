/******************************************************************************
 * Copyright 2024-present, Joseph Garnier
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 ******************************************************************************/

#include "window.h"

namespace FastSimDesign {

  Window::Window(std::string title) noexcept
    : m_title{std::move(title)}
  {
  }
}