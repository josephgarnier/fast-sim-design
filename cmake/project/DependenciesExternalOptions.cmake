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
	set(Qt5_DIR "/opt/Qt/5.15.2/gcc_64/lib/cmake/Qt5")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(Qt5_DIR "/opt/Qt/5.15.2/gcc_64/lib/cmake/Qt5")
endif()
if(DEFINED ENV{CMAKE_PREFIX_PATH}) 
	set(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")
else()
	set(CMAKE_PREFIX_PATH "${Qt5_DIR}")
endif()

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)
set(CMAKE_AUTOUIC on)

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

# Link Qt to the main binary build target.
message(STATUS "Link Qt to the target \"${${PROJECT_NAME}_MAIN_BIN_TARGET}\"")
target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
		"$<INSTALL_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
)

# Set Qt as a position-independent target.
set_target_properties("${${PROJECT_NAME}_MAIN_BIN_TARGET}" PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON)
if(${${PROJECT_NAME}_MAIN_BIN_TARGET_IS_EXEC})
	target_compile_options("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
		PUBLIC
			"$<BUILD_INTERFACE:-fPIE>"
			"$<INSTALL_INTERFACE:-fPIE>"
		PRIVATE
			"-fPIE"
	)
elseif(${${PROJECT_NAME}_MAIN_BIN_TARGET_IS_STATIC} OR ${${PROJECT_NAME}_MAIN_BIN_TARGET_IS_SHARED})
	target_compile_options("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:-fPIC>"
		"$<INSTALL_INTERFACE:-fPIC>"
	PRIVATE
		"-fPIC"
	)
endif()

# Add Qt assert definitions to target if needed (these instructions are optional,
# they are only a way to easily enable or disable asserts from the file `cmake/project/StandardOptions.txt`.
if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
	message(STATUS "QtAssert enabled")
else()
	message(STATUS "Add Qt assert definitions to target")
	target_compile_definitions("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
		PUBLIC
			"$<BUILD_INTERFACE:QT_NO_DEBUG>"
			"$<INSTALL_INTERFACE:QT_NO_DEBUG>"
	)
	message(STATUS "QtAssert disabled")
endif()

message(STATUS "Import and link Qt - done")
return()



set(QOBJECT_SOURCE_FILES
	"${${PROJECT_NAME}_SRC_DIR}/config/preferences.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/entity/entity.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/entity/npc.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/entity/object.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/entity/entity_storage.cpp"
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
	"${${PROJECT_NAME}_SRC_DIR}/entity/entity_storage.h"
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

return()
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
target_sources("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PRIVATE
		"${RELATIVE_QOBJECT_SOURCE_FILES};${RELATIVE_MOC_HEADER_FILES};${RELATIVE_UI_SOURCE_FILES};${RELATIVE_RESSOURCE_SRCS}"
)

# Add Qt definitions to target
message(STATUS "Add Qt definitions to target")
target_compile_definitions("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:QT_USE_QSTRINGBUILDER;QT_SHAREDPOINTER_TRACK_POINTERS;QT_MESSAGELOGCONTEXT>"
		"$<INSTALL_INTERFACE:QT_USE_QSTRINGBUILDER;QT_SHAREDPOINTER_TRACK_POINTERS;QT_MESSAGELOGCONTEXT>"
)

# Link Qt to target
message(STATUS "Link Qt to target")
target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
		"$<INSTALL_INTERFACE:Qt5::Widgets;Qt5::Gui;Qt5::Core;Qt5::Svg;Qt5::Concurrent>"
)

# Set Qt as a position-independent target
set_target_properties("${${PROJECT_NAME}_MAIN_BIN_TARGET}" PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON)
if(${${PROJECT_NAME}_MAIN_BIN_TARGET_IS_EXEC})
	target_compile_options("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
		PUBLIC
			"$<BUILD_INTERFACE:-fPIE>"
			"$<INSTALL_INTERFACE:-fPIE>"
		PRIVATE
			"-fPIE"
	)
elseif(${${PROJECT_NAME}_MAIN_BIN_TARGET_IS_STATIC} OR ${${PROJECT_NAME}_MAIN_BIN_TARGET_IS_SHARED})
	target_compile_options("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PUBLIC
		"$<BUILD_INTERFACE:-fPIC>"
		"$<INSTALL_INTERFACE:-fPIC>"
	PRIVATE
		"-fPIC"
	)
endif()

# Add Qt assert definitions to target if needed (these instructions are optional,
# they are only a way to easily enable or disable asserts from the file `cmake/project/StandardOptions.txt`.
if("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
	message(STATUS "QtAssert enabled")
else()
	message(STATUS "Add Qt assert definitions to target")
	target_compile_definitions("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
		PUBLIC
			"$<BUILD_INTERFACE:QT_NO_DEBUG>"
			"$<INSTALL_INTERFACE:QT_NO_DEBUG>"
	)
	message(STATUS "QtAssert disabled")
endif()