# Fast-Sim-Design

</div>
<p align="center">
<strong>Design your world with <a href="https://www.mapeditor.org/">Tiled</a>, make it alive with FastSimDesign</strong>
</p>

<p align="center">
<a rel="license" href="https://www.gnu.org/licenses/gpl-3.0.en.html"><img alt="Static Badge" src="https://img.shields.io/badge/licence-GNU_GPLv3-brightgreen">
</a> <img alt="Static Badge" src="https://img.shields.io/badge/plateform-Windows%20%7C%20Linux%20%7C%20Mac-lightgrey"> <img alt="Static Badge" src="https://img.shields.io/badge/language-C%2B%2B%20%7C%20CMake-blue"> <img alt="Static Badge" src="https://img.shields.io/badge/status-in_dev-orange">
</p>

Fast-Sim-Design is an agent-based virtual world simulator in C++ for maps created with [Tiled software](https://www.mapeditor.org/). It use the [SFML engine](https://www.sfml-dev.org/) to simulate the world and [Dear ImGui](https://github.com/ocornut/imgui)/[ImGui-SFML](https://github.com/SFML/imgui-sfml) to display what's happening in the simulation. It is designed to get design projects off the ground quickly, and offers several tools for visualizing activity within the simulator to facilitate analysis. This makes Fast-Sim-Design ideal for simulation experiments. Finally, particular attention was paid to code quality, documentation and unit testing to guarantee reliable behavior.

⚠️ **Development in progress** ⚠️

## 💠 Table of Contents <!-- omit in toc -->

- [✨ Features](#-features)
- [⚓ Requirements](#-requirements)
- [🚀 Getting started](#-getting-started)
- [💫 Architecture overview](#-architecture-overview)
- [📄 Setting up the environment](#-setting-up-the-environment)
- [⚙️ Usage and commands](#️-usage-and-commands)
- [📂 Folder structure overview](#-folder-structure-overview)
- [❓ How-To](#-how-to)
  - [Update the Tiled library](#update-the-tiled-library)
- [🤝 Contributing](#-contributing)
- [👥 Credits](#-credits)
- [©️ License](#️-license)
- [🍻 Acknowledgments](#-acknowledgments)

## ✨ Features

- Agent modeling with the Entity-component-system (ECS).
- A fast and lightweight simulator thanks to SFML.
- An analysis of the state of simulation powered by Dear ImGui.
- Modern code, written in C++ 20 and modern CMake.
- Reliable code thanks to extensive unit testing and the implementation of best practices.

## ⚓ Requirements

The following dependencies are **required** for development and must be installed:

- **C++20 compiler** - e.g [GCC v15.2+](https://gcc.gnu.org/), [Clang C++ v19.1.3+](https://clang.llvm.org/cxx_status.html) or [Visual Studio](https://visualstudio.microsoft.com). The project is developed with the GCC compiler, and its dependencies are provided pre-compiled with GCC.
- **CMake v3.31.2+** - can be found [here](https://cmake.org/).

The following dependencies are **required** for development, but their installation is **optional** because they will be **automatically downloaded** by CMake if they cannot be found:

- **SFML v2.6.2+** for simulation engine - can be found [here](https://www.sfml-dev.org/download.php) (the Linux version has [some requirements](https://www.sfml-dev.org/tutorials/2.6/start-cmake.php) for autodownloading).
- **Dear ImGui v1.91.6+** for GUI - can be found [here](https://github.com/ocornut/imgui).
- **ImGui-SFML v2.6+** for Dear ImGui + SFML - can be found [here](https://github.com/SFML/imgui-sfml).
- **spdlog v1.15.0+** for logging - can be found [here](https://github.com/gabime/spdlog).

The following dependencies are **optional** and depends on project configuration. Their installation is optional because they will be **automatically downloaded** by CMake if they can't be found:

- [Doxygen v1.12+](http://www.doxygen.nl/) (used when `ENABLE_TEST_MODULE` option is set to `on`);
- [GTest v1.15.2+](https://github.com/google/googletest) (used when `ENABLE_DOC_MODULE` option is set to `on`).

In addition, it is recommended **to install [Tiled](https://www.mapeditor.org/)** to design your own map to test Fast-Sim-Design.

## 🚀 Getting started

## 💫 Architecture overview

## 📄 Setting up the environment

## ⚙️ Usage and commands

This project provide several scripts and commands to generate the *Build Lifecycle* and execute each build phase with their targets. If you are a VS Code user, they have all been written in `.vscode/tasks.json` and can be launched from the [command palette](https://code.visualstudio.com/docs/editor/tasks), otherwise you can use a command prompt. All the following instructions have to be executed from the root of the project. They are listed in the order of execution of a complete and classic sequence of build phases.

Commands to **clean the *Build Lifecycle*** (these scripts clean `build/`, `doc/` and `bin/`):

```bash
# clean the Build Lifecycle (on Linux/MacOS)
./clean-cmake.sh

# clean the Build Lifecycle (on Windows)
clean-cmake.bat
```

Commands to **generate the *Build Lifecycle*** (these scripts call the `cmake` command):

```bash
# generate the Build Lifecycle (on Linux/MacOS)
./run-cmake.sh

# generate the Build Lifecycle (on Windows)
run-cmake.bat

# a useful command for listing what targets has been generated
cmake --build build/ --target help

# a useful command for listing variables in the cache and their descriptions
cmake -LAH build/
```

Commands to **clean and generate the *Build Lifecycle***:

```bash
# clean and generate the Build Lifecycle (on Linux/MacOS)
./clean-cmake.sh && sleep 3s && echo \"\" && ./run-cmake.sh

# clean and generate the Build Lifecycle (on Windows)
clean-cmake.bat && timeout /t 3 > NUL && echo. && run-cmake.bat
```

Commands to **execute the `uninstall` build phase** of the *Build Lifecycle* (only available if the *Export Generator Module* has been activated):

```bash
# run the uninstall target (on Linux/MacOS)
sudo cmake --build build/ --target uninstall

# run the uninstall target (on Windows)
cmake --build build/ --target uninstall
```

Commands to **execute the `clean` build phase** of the *Build Lifecycle*:

```bash
# run the clean target
cmake --build build/ --target clean
```

Commands to **execute the `compile` build phase** of the *Build Lifecycle*:

```bash
# build all binary targets (except for tests)
cmake --build build/ --target all

# build all binary targets in verbose mode (except for tests)
cmake --build build/ --target all --verbose

# execute the `compile` phase after the `clean` phase
cmake --build build/ --target all --clean-first

# execute the `compile` phase after the `clean` phase in verbose
cmake --build build/ --target all --clean-first --verbose
```

Commands to **execute the `test` build phase** of the *Build Lifecycle* (only available if the *Test Generator Module* has been activated):

```bash
# build the test binary target and execute the tests binary executable
cmake --build build/ --target project-name_test && ../bin/project-name_test
```

Commands to **execute the `doc` build phase** of the *Build Lifecycle* (only available if the *Doc Generator Module* has been activated):

```bash
# run the doc target
cmake --build build/ --target doc
```

Commands to **execute the `install` build phase** of the *Build Lifecycle* (only available if the *Export Generator Module* has been activated):

```bash
# run the install target (on Linux/MacOS)
sudo cmake --build build/ --target install

# run the install target (on Windows)
cmake --build build/ --target install
```

Commands to **execute the `package` build phase** of the *Build Lifecycle* (only available if the *Package Generator Module* has been activated):

```bash
# run the package and package_source targets (on Linux/MacOS)
cmake --build build/ --target package package_source && sleep 3s && rm -rfv bin/_CPack_Packages

# run the package and package_source targets (on Windows)
cmake --build build/ --target package package_source && timeout /t 3 > NUL && del /a /f /s /q bin/_CPack_Packages

# run the package target (on Linux/MacOS)
cmake --build build/ --target package && sleep 3s && rm -rfv bin/_CPack_Packages

# run the package target (on Windows)
cmake --build build/ --target package && timeout /t 3 > NUL && del /a /f /s /q bin/_CPack_Packages

# run the package_source target (on Linux/MacOS)
cmake --build build/ --target package_source && sleep 3s && rm -rfv bin/_CPack_Packages

# run the package_source target (on Windows)
cmake --build build/ --target package_source && timeout /t 3 > NUL && del /a /f /s /q bin/_CPack_Packages

# a useful command for debugging during the package configuration step (on Linux/MacOS)
cpack --debug --verbose --config build/CPackConfig.cmake && sleep 3s && rm -rfv bin/_CPack_Packages

# a useful command for debugging during the package configuration step (on Windows)
cpack --debug --verbose --config build/CPackConfig.cmake && timeout /t 3 > NUL && del /a /f /s /q bin/_CPack_Packages
```

Use the following commands to **execute the binaries built** as executable:

```bash
# execute the main binary executable (on Linux/MacOS)
./bin/project-name

# execute the main binary executable (on Windows)
bin/project-name
```

## 📂 Folder structure overview

This project has been set up with a specific file/folder structure in mind. The following describes some important features of this setup:

| **Directory and File** | **What belongs here** |
|------------------------|-----------------------|
| `.vscode/tasks.json` | Specific VS Code tasks configured to compile, clean, build, etc. |
| `assets/` | Contains images, musics, maps and all resources needed for a game or a simulation project. |
| `bin/` | Any libs that get compiled by the project and the output executables go here, also if you pack your project, the generated files go here. |
| `build/` | Contains the CMake build-tree. |
| `cmake/helpers/` | Contains some scripts and all generator modules used to generate the *Build Lifecycle*. |
| `cmake/modules/` | Contains custom CMake modules. |
| `cmake/project/` | Setting files for configuring the generator modules. |
| `cmake/toolchains/` | Contains toolchain files for compilers.
| `config/` | Contains configuration files used by the C++ project. |
| `doc/` | Contains code documentation generated by [Doxygen](http://www.doxygen.org). |
| `include/` | All necessary third-party header files (.h) and public header files (.h) of the project. |
| `lib/` | Any libaries needed in the project. |
| `resources/` | Contains images, musics, maps and all resources needed for the project (e.g for graphical user interfaces). |
| `src/` | Source files (.cpp) and private/public header files (.h) of the project. |
| `tests/` | Source files (.cpp) and header files (.h) for the unit testing framework [GTest](https://github.com/google/googletest). |
| `clean-cmake.bat` | Utility script for Windows to remove all generated files in `build/`, `bin/` and `doc/`. |
| `clean-cmake.sh` | Utility script for Linux/MacOS to remove all generated files in `build/`, `bin/` and `doc/` directories. |
| `CMakeLists.txt` | Main `CMakelists.txt` file of the project. |
| `LICENSE.md` | License file for project (needs to be edited). |
| `README.md` | Readme file for project (needs to be edited). |
| `run-cmake.bat` | Utility script for Windows to generate the *Build Lifecycle*. |
| `run-cmake.sh` | Utility script for Linux/MacOS to generate the *Build Lifecycle*. |

## ❓ How-To

### Update the Tiled library

Fast-Sim-Design is supplied with the Tiled library already pre-compiled with GCC. To rebuild and update it, proceed as follows:

1. Make sure that the Qt Creator software and the Qt framework compiled with GCC are installed.
2. Using a Web browser, go to the [Tiled release download page](https://github.com/mapeditor/tiled/releases).
3. Find the section corresponding to the version of Tiled you want or have already installed on your computer, then click on **Source code** to download the source code.
4. Extract the contents of the downloaded archive into the folder of your choice.
5. Launch Qt Creator and open the tiled project located in the extracted folder.
6. Open the file `Tield/libtiled/libtiled.qbs`.
7. To change the name of the generated target according to the build type, replace the `targetName: ...` line with `qbs.buildVariant === "debug" ? "tiledd_x.xx.x" : "tiled_x.xx.x"`, substituting the `x` with the current Tiled version number, then save the file.
8. Start compiling the library from the `libtiled` folder (right-click on the folder, then select **Build**).
9. When the operation is complete, compile the other build type: *debug* or *release*.

At the end of the procedure, the compiled library is available from the Qt Creator output folders: `<path-to-project-tield>/build/<Release|Debug>_Desktop/libtiled/<tiled_x.xx.x|tiledd_x.xx.x>`. Copy them into the project's `lib/` folder. Also copy Tiled's C++ header files from the `<path-to-project-tield>/libtiled/` folder to the `include/tiled/` project folder.

## 🤝 Contributing

1. Fork the repo and create your feature branch from master.
2. Create a topic branch - `git checkout -b my_branch`.
3. Push to your branch - `git push origin my_branch`.
4. Create a Pull Request from your branch.

## 👥 Credits

This project is maintained and developed by [Joseph Garnier](https://www.joseph-garnier.fr/).

## ©️ License

This work is licensed under the terms of a <a href="https://www.gnu.org/licenses/gpl-3.0.en.html" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">GNU GPLv3</a>.  See the [LICENSE.md](LICENSE.md) file for details.

## 🍻 Acknowledgments

This project was inspired by [Tiled](https://www.mapeditor.org/).
