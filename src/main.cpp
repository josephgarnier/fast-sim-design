////////////////////////////////////////////////////////////
///
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#include <iostream>
#include "core/application.h"
#include "core/log.h"

int main(int argc, char* argv[])
{
  try
  {
    FastSimDesign::Log::init();
    FastSimDesign::Application::launch(FastSimDesign::Application{});
    FastSimDesign::Log::shutdown();
  } catch (std::exception const& e)
  {
    std::cout << "\nException: " << e.what() << std::endl;
  }

  return 0;
}
