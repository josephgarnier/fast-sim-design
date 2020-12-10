# Copyright 2019-present, Joseph Garnier
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.
# =============================================================================
# What Is This?
# -------------
# Put in this file all instructions to include and link your library dependencies
# to your own library and own executable. All libraries (.dll or .so files) found by the
# `directory()` function will be automatically link, but to include their header
# file, you have to add this function. For each directory in `include/`,
# except for this containing your own headers, write the following instructions at
# the end of this file in remplacing `<library-name-directory>` by the name of
# directory where your library is:
# 
# set(${PROJECT_NAME}_LIBRARY_HEADER_DIRS "${${PROJECT_NAME}_INCLUDE_DIR}/<library-name-directory>")
#
# Warning: according to the soname policy on Linux (https://en.wikipedia.org/wiki/Soname),
# don't forget to create a link for each library in `lib\`.
#
# On the contrary, if you want to use an external library (e.g Qt) in using
# `find_package()` function, you don't need the previous code, but rather have
# to add your special instructions like `find_package()`, `target_sources()`,
# `target_include_directories()`, target_compile_definitions()` and
# `target_link_libraries()` at the end of file. You have to add these properties on
# the target : `${${PROJECT_NAME}_TARGET_NAME}`.
# To know how import a such library please read its documentation.
# Last thing, this is in this file that you will use the parameter `DPARAM_ASSERT_ENABLE`
# with a test like `if(${PARAM_ASSERT_ENABLE})`.
# An illustrated example for Qt, which you will have to delete, is proposed at the
# end of the file.
#
# Warning: if you use `find_package()` function, then don't forget to add
# your dependancies in the file cmake/project/PackageConfig.cmake.

include(Directory)
include(FileManip)

set(${PROJECT_NAME}_LIBRARY_FILES "")
directory(SCAN ${PROJECT_NAME}_LIBRARY_FILES
	LIST_DIRECTORIES off
	RELATIVE off
	ROOT_DIR "${${PROJECT_NAME}_LIB_DIR}"
	INCLUDE_REGEX ".*\\${CMAKE_SHARED_LIBRARY_SUFFIX}.*|.*\\${CMAKE_STATIC_LIBRARY_SUFFIX}.*"
)

# First use case: you want to use the internal and automatic mechanism of library integration.
# Append each include directories of your libraries in this list
# (in this way `${${PROJECT_NAME}_INCLUDE_DIR}/<library-name-directory>`) or
# let it empty. They will be added to include directories of target and copied
# by `install()` command.
#  ||
#  V
set(${PROJECT_NAME}_LIBRARY_HEADER_DIRS "${${PROJECT_NAME}_INCLUDE_DIR}/tiled" "${${PROJECT_NAME}_INCLUDE_DIR}/zlib")


# Second use case : you want to link a library installed in another folder than the one of your project.
# Add your special instructions here.
#  ||
#  V
# Include Qt
message("\n** Include Qt **")
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
find_package(Qt5 COMPONENTS Widgets Gui Core Svg Concurrent REQUIRED)
find_package(Qt5CoreMacrosCustom REQUIRED)
find_package(Qt5WidgetsMacrosCustom REQUIRED)

if (${Qt5Widgets_VERSION} VERSION_LESS 5.15.2
	OR ${Qt5Gui_VERSION} VERSION_LESS 5.15.2
	OR ${Qt5Core_VERSION} VERSION_LESS 5.15.2
	OR ${Qt5Svg_VERSION} VERSION_LESS 5.15.2
	OR ${Qt5Concurrent_VERSION} VERSION_LESS 5.15.2)
		message(FATAL_ERROR "Minimum supported Qt5 version is 5.15.2!")
endif()

set(QOBJECT_SOURCE_FILES
	"${${PROJECT_NAME}_SRC_DIR}/config/preferences.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/entity/entity.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/entity/npc.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/entity/object.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/gui/dock/map/map_scene.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/gui/dock/map/map_view.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/gui/command_state_machine.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/gui/simulator_gui.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/level/world_info_model.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/level/world.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/resource/file_resource.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/resource/map_resource.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/resource/resource.cpp"
)
set(QOBJECT_HEADER_FILES
	"${${PROJECT_NAME}_SRC_DIR}/config/preferences.h"
	"${${PROJECT_NAME}_SRC_DIR}/entity/entity.h"
	"${${PROJECT_NAME}_SRC_DIR}/entity/npc.h"
	"${${PROJECT_NAME}_SRC_DIR}/entity/object.h"
	"${${PROJECT_NAME}_SRC_DIR}/gui/dock/map/map_scene.h"
	"${${PROJECT_NAME}_SRC_DIR}/gui/dock/map/map_view.h"
	"${${PROJECT_NAME}_SRC_DIR}/gui/command_state_machine.h"
	"${${PROJECT_NAME}_SRC_DIR}/gui/simulator_gui.h"
	"${${PROJECT_NAME}_SRC_DIR}/level/world_info_model.h"
	"${${PROJECT_NAME}_SRC_DIR}/level/world.h"
	"${${PROJECT_NAME}_SRC_DIR}/resource/file_resource.h"
	"${${PROJECT_NAME}_SRC_DIR}/resource/map_resource.h"
	"${${PROJECT_NAME}_SRC_DIR}/resource/resource.h"
	
)
set(UI_FILES 
	"${${PROJECT_NAME}_SRC_DIR}/gui/simulator_gui.ui"
)
set(RESSOURCE_FILES
	"${${PROJECT_NAME}_RESOURCES_DIR}/resources.qrc"
)

# The directory where the files will be generated should be added to the
# variable `${PROJECT_NAME}_HEADER_PUBLIC_DIRS` in `ProjectSrcFiles.cmake`.
qt5_wrap_cpp(MOC_HEADER_FILES ${QOBJECT_HEADER_FILES})
qt5_wrap_ui_custom(UI_SOURCE_FILES ${UI_FILES})
qt5_add_resources_custom(RESSOURCE_SRCS ${RESSOURCE_FILES})

set(RELATIVE_QOBJECT_SOURCE_FILES "")
file_manip(RELATIVE_PATH QOBJECT_SOURCE_FILES BASE_DIR "${${PROJECT_NAME}_PROJECT_DIR}" OUTPUT_VARIABLE RELATIVE_QOBJECT_SOURCE_FILES)
set(RELATIVE_QOBJECT_HEADER_FILES "")
file_manip(RELATIVE_PATH QOBJECT_HEADER_FILES BASE_DIR "${${PROJECT_NAME}_PROJECT_DIR}" OUTPUT_VARIABLE RELATIVE_QOBJECT_HEADER_FILES)
set(RELATIVE_MOC_HEADER_FILES "")
file_manip(RELATIVE_PATH MOC_HEADER_FILES BASE_DIR "${${PROJECT_NAME}_PROJECT_DIR}" OUTPUT_VARIABLE RELATIVE_MOC_HEADER_FILES)
set(RELATIVE_UI_FILES "")
file_manip(RELATIVE_PATH UI_FILES BASE_DIR "${${PROJECT_NAME}_PROJECT_DIR}" OUTPUT_VARIABLE RELATIVE_UI_FILES)
set(RELATIVE_UI_SOURCE_FILES "")
file_manip(RELATIVE_PATH UI_SOURCE_FILES BASE_DIR "${${PROJECT_NAME}_PROJECT_DIR}" OUTPUT_VARIABLE RELATIVE_UI_SOURCE_FILES)
set(RELATIVE_RESSOURCE_FILES "")
file_manip(RELATIVE_PATH RESSOURCE_FILES BASE_DIR "${${PROJECT_NAME}_PROJECT_DIR}" OUTPUT_VARIABLE RELATIVE_RESSOURCE_FILES)
set(RELATIVE_RESSOURCE_SRCS "")
file_manip(RELATIVE_PATH RESSOURCE_SRCS BASE_DIR "${${PROJECT_NAME}_PROJECT_DIR}" OUTPUT_VARIABLE RELATIVE_RESSOURCE_SRCS)

message(STATUS "QObject sources found:")
foreach(file IN ITEMS ${RELATIVE_QOBJECT_SOURCE_FILES})
	message("    ${file}")
endforeach()

message(STATUS "QObject headers found:")
foreach(file IN ITEMS ${RELATIVE_QOBJECT_HEADER_FILES})
	message("    ${file}")
endforeach()

message(STATUS "QObject moc found:")
foreach(file IN ITEMS ${RELATIVE_MOC_HEADER_FILES})
	message("    ${file}")
endforeach()

message(STATUS "UI files found:")
foreach(file IN ITEMS ${RELATIVE_UI_FILES})
	message("    ${file}")
endforeach()

message(STATUS "UI sources found:")
foreach(file IN ITEMS ${RELATIVE_UI_SOURCE_FILES})
	message("    ${file}")
endforeach()

message(STATUS "Ressources files found:")
foreach(file IN ITEMS ${RELATIVE_RESSOURCE_FILES})
	message("    ${file}")
endforeach()

message(STATUS "Ressources sources found:")
foreach(file IN ITEMS ${RELATIVE_RESSOURCE_SRCS})
	message("    ${file}")
endforeach()
message("")

# Add Qt sources to target
message(STATUS "Add Qt sources to target")
target_sources("${${PROJECT_NAME}_TARGET_NAME}"
	PRIVATE
		"${RELATIVE_QOBJECT_SOURCE_FILES};${RELATIVE_MOC_HEADER_FILES};${RELATIVE_UI_SOURCE_FILES};${RELATIVE_RESSOURCE_SRCS}"
)

# Add Qt definitions to target
message(STATUS "Add Qt definitions to target")
target_compile_definitions("${${PROJECT_NAME}_TARGET_NAME}"
	PUBLIC
		"$<BUILD_INTERFACE:QT_USE_QSTRINGBUILDER;QT_SHAREDPOINTER_TRACK_POINTERS;QT_MESSAGELOGCONTEXT>"
		"$<INSTALL_INTERFACE:QT_USE_QSTRINGBUILDER;QT_SHAREDPOINTER_TRACK_POINTERS;QT_MESSAGELOGCONTEXT>"
)

# Link Qt to target
message(STATUS "Link Qt to target\n")
target_link_libraries("${${PROJECT_NAME}_TARGET_NAME}"
	PUBLIC
		"$<BUILD_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
		"$<INSTALL_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
)

# Set Qt as a position-independent target
set_target_properties("${${PROJECT_NAME}_TARGET_NAME}" PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON)
if(${${PROJECT_NAME}_TARGET_IS_EXEC})
	target_compile_options("${${PROJECT_NAME}_TARGET_NAME}"
		PUBLIC
			"$<BUILD_INTERFACE:-fPIE>"
			"$<INSTALL_INTERFACE:-fPIE>"
		PRIVATE
			"-fPIE"
	)
elseif(${${PROJECT_NAME}_TARGET_IS_STATIC} OR ${${PROJECT_NAME}_TARGET_IS_SHARED})
	target_compile_options("${${PROJECT_NAME}_TARGET_NAME}"
	PUBLIC
		"$<BUILD_INTERFACE:-fPIC>"
		"$<INSTALL_INTERFACE:-fPIC>"
	PRIVATE
		"-fPIC"
	)
endif()

# Add Qt assert definitions to target if needed
if(${PARAM_ASSERT_ENABLE})
	message(STATUS "QtAssert enabled\n")
else()
	message(STATUS "Add Qt assert definitions to target")
	target_compile_definitions("${${PROJECT_NAME}_TARGET_NAME}"
		PUBLIC
			"$<BUILD_INTERFACE:QT_NO_DEBUG>"
			"$<INSTALL_INTERFACE:QT_NO_DEBUG>"
	)
	message(STATUS "QtAssert disabled\n")
endif()
