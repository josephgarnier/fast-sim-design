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
#include "../utils/generic_utility.h"

namespace FastSimDesign {
  Command::Command() noexcept
    : action{}
    , category{toUnderlyingType(Category::Type::NONE)}
  {
  }
}