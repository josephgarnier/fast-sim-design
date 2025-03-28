////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "command.h"
#include "../entity/category.h"

namespace FastSimDesign {
  Command::Command() noexcept
    : name{""}
    , category{Category::Type::NONE}
    , action{}
  {
  }
}