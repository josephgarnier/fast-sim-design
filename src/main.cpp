////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include "core/application.h"
#include "core/log.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  try
  {
    FastSimDesign::Log::init();
    FastSimDesign::Application::launch(FastSimDesign::Application{});
    FastSimDesign::Log::shutdown();
  }
  catch (std::exception const& e)
  {
    std::cout << "\nException: " << e.what() << std::endl;
  }

  return 0;
}
