# Copyright 2019-present, Joseph Garnier
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.
# =============================================================================
# What Is This?
# -------------
# See README file in the root directory of this source tree.


#------------------------------------------------------------------------------
# Import and link external libraries from here.
#------------------------------------------------------------------------------

#---- Import and link Qt. ----
message(STATUS "Import and link Qt")
if(DEFINED ENV{Qt5_DIR}) 
	set(Qt5_DIR "$ENV{Qt5_DIR}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	set(Qt5_DIR "D:/Documents/Travail/Libraries/Qt/5.15.2/mingw81_64/lib/cmake")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(Qt5_DIR "/opt/Qt/5.15.2/gcc_64/lib/cmake")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(Qt5_DIR "/opt/Qt/5.15.2/gcc_64/lib/cmake")
endif()
if(DEFINED ENV{CMAKE_PREFIX_PATH}) 
	set(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")
else()
	set(CMAKE_PREFIX_PATH "${Qt5_DIR}")
endif()

# See https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html for their documentations.
set_target_properties("${${PROJECT_NAME}_MAIN_BIN_TARGET}" PROPERTIES
	AUTOGEN_ORIGIN_DEPENDS on
	AUTOMOC on
	AUTOMOC_COMPILER_PREDEFINES on
	AUTOMOC_MACRO_NAMES "${CMAKE_AUTOMOC_MACRO_NAMES}"
	AUTOMOC_PATH_PREFIX on
	AUTORCC on
	AUTOUIC on
	AUTOUIC_SEARCH_PATHS "${${PROJECT_NAME}_SRC_DIR}/gui"
)

find_package(Qt5 COMPONENTS Widgets Gui Core Svg Concurrent REQUIRED)

if (${Qt5Widgets_VERSION} VERSION_LESS 5.15.1
	OR ${Qt5Gui_VERSION} VERSION_LESS 5.15.1
	OR ${Qt5Core_VERSION} VERSION_LESS 5.15.1
	OR ${Qt5Svg_VERSION} VERSION_LESS 5.15.1
	OR ${Qt5Concurrent_VERSION} VERSION_LESS 5.15.1)
		message(FATAL_ERROR "Minimum supported Qt5 version is 5.15.1!")
endif()

# Add Qt definitions to the main binary build target.
message(STATUS "Add Qt definitions to the target \"${${PROJECT_NAME}_MAIN_BIN_TARGET}\"")
target_compile_definitions("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:QT_USE_QSTRINGBUILDER;QT_SHAREDPOINTER_TRACK_POINTERS;QT_MESSAGELOGCONTEXT>"
		"$<INSTALL_INTERFACE:QT_USE_QSTRINGBUILDER;QT_SHAREDPOINTER_TRACK_POINTERS;QT_MESSAGELOGCONTEXT>"
)
# Add Qt assert definitions to the main binary build target only for debug.
target_compile_definitions("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:$<$<NOT:$<STREQUAL:${CMAKE_BUILD_TYPE},DEBUG>>:QT_NO_DEBUG>>"
		"$<INSTALL_INTERFACE:$<$<NOT:$<STREQUAL:${CMAKE_BUILD_TYPE},DEBUG>>:QT_NO_DEBUG>>"
)

# Link Qt to the main binary build target.
message(STATUS "Link Qt to the target \"${${PROJECT_NAME}_MAIN_BIN_TARGET}\"")
target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
		"$<INSTALL_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
)

# Set Qt as a position-independent target.
set_target_properties("${${PROJECT_NAME}_MAIN_BIN_TARGET}" PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON)
target_compile_options("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:$<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>,-fPIE,-fPIC>>"
		"$<INSTALL_INTERFACE:$<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>,-fPIE,-fPIC>>"
	PRIVATE
		"$<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>,-fPIE,-fPIC>"
)
message(STATUS "Import and link Qt - done")
