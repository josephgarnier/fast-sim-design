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

#---- Import and link SFML. ----
message(STATUS "Import and link SFML")
if(DEFINED ENV{SFML_DIR}) 
	set(SFML_DIR "$ENV{SFML_DIR}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(SFML_DIR "D:/Documents/Software_Libraries/SFML-2.6.2/vs17-w64")
	else()
		set(SFML_DIR "D:/Documents/Software_Libraries/SFML-2.6.2/gcc-13.1.0-mingw64")
	endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(SFML_DIR "/opt/SFML-2.6.2")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(SFML_DIR "/opt/SFML-2.6.2")
endif()
if(DEFINED ENV{CMAKE_PREFIX_PATH}) 
	set(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")
else()
	set(CMAKE_PREFIX_PATH "${SFML_DIR}")
endif()

# Find SFML or auto-download it.
message(STATUS "Find SFML package")
find_package(SFML COMPONENTS system graphics window audio network REQUIRED)
if(NOT ${SFML_FOUND})
	message(STATUS "SFML not found, it will be auto-downloaded in the build-tree")
	include(FetchContent)
	set(FETCHCONTENT_QUIET off)
	FetchContent_Declare(
		SFML
		GIT_REPOSITORY https://github.com/SFML/SFML.git
		GIT_TAG 2.6.2
		GIT_SHALLOW on
		GIT_PROGRESS on
		EXCLUDE_FROM_ALL
		SYSTEM
		STAMP_DIR "${${PROJECT_NAME}_BUILD_DIR}"
		DOWNLOAD_NO_PROGRESS off
		LOG_DOWNLOAD on
		LOG_UPDATE on
		LOG_PATCH on
		LOG_CONFIGURE on
		LOG_BUILD on
		LOG_INSTALL on
		LOG_TEST on
		LOG_MERGED_STDOUTERR on
		LOG_OUTPUT_ON_FAILURE on
		USES_TERMINAL_DOWNLOAD on
	)
	FetchContent_MakeAvailable(SFML)
	if(WIN32)
		add_custom_command(
			TARGET "${${PROJECT_NAME}_MAIN_BIN_TARGET}"
			COMMENT "Copy OpenAL DLL"
			PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${${PROJECT_NAME}_MAIN_BIN_TARGET}>
			VERBATIM
		)
	endif()
else()
  message(STATUS "SFML found")
endif()

# Link SFML to the main binary build target.
message(STATUS "Link SFML library to the target \"${${PROJECT_NAME}_MAIN_BIN_TARGET}\"")
target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PRIVATE
		"sfml-system;sfml-graphics;sfml-window;sfml-audio;sfml-network"
)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set_target_properties("${${PROJECT_NAME}_MAIN_BIN_TARGET}" PROPERTIES
		VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;D:/Documents/Software_Libraries/SFML-2.6.2/vs17-w64/bin"
	)
endif()
message(STATUS "Import and link SFML - done")


#---- Import and link Dear ImGui. ----
message(STATUS "Import and link Dear ImGui")
if(DEFINED ENV{IMGUI_DIR}) 
	set(IMGUI_DIR "$ENV{IMGUI_DIR}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	set(IMGUI_DIR "D:/Documents/Software_Libraries/ImGui/1.91.6")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(IMGUI_DIR "/opt/ImGui/1.91.6")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(IMGUI_DIR "/opt/ImGui/1.91.6")
endif()
if(DEFINED ENV{CMAKE_PREFIX_PATH}) 
	set(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")
else()
	set(CMAKE_PREFIX_PATH "${IMGUI_DIR}")
endif()

# Find Dear ImGui or auto-download it.
message(STATUS "Find Dear ImGui package")
find_package(imgui NO_MODULE)
if(NOT ${imgui_FOUND})
	message(STATUS "Dear ImGui not found, it will be auto-downloaded in the build-tree")
	include(FetchContent)
	set(FETCHCONTENT_QUIET off)
	FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG v1.91.6-docking
		GIT_SHALLOW on
		GIT_PROGRESS on
		EXCLUDE_FROM_ALL
		SYSTEM
		STAMP_DIR "${${PROJECT_NAME}_BUILD_DIR}"
		DOWNLOAD_NO_PROGRESS off
		LOG_DOWNLOAD on
		LOG_UPDATE on
		LOG_PATCH on
		LOG_CONFIGURE on
		LOG_BUILD on
		LOG_INSTALL on
		LOG_TEST on
		LOG_MERGED_STDOUTERR on
		LOG_OUTPUT_ON_FAILURE on
		USES_TERMINAL_DOWNLOAD on
	)
	FetchContent_MakeAvailable(imgui)
	set(IMGUI_DIR "${imgui_SOURCE_DIR}")
else()
  message(STATUS "Dear ImGui found")
endif()

# Link Dear ImGui to the main binary build target.
message(STATUS "No need to link Dear ImGui library to the target \"${${PROJECT_NAME}_MAIN_BIN_TARGET}\"")
message(STATUS "Import and link Dear ImGui - done")


#---- Import and link ImGui-SFML. ----
message(STATUS "Import and link ImGui-SFML")
if(DEFINED ENV{IMGUI-SFML_DIR}) 
	set(IMGUI-SFML_DIR "$ENV{IMGUI-SFML_DIR}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	set(IMGUI-SFML_DIR "D:/Documents/Software_Libraries/ImGui-SFML/1.91.6")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(IMGUI-SFML_DIR "/opt/ImGui-SFML/1.91.6")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(IMGUI-SFML_DIR "/opt/ImGui-SFML/1.91.6")
endif()
if(DEFINED ENV{CMAKE_PREFIX_PATH}) 
	set(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")
else()
	set(CMAKE_PREFIX_PATH "${IMGUI-SFML_DIR}")
endif()

# Set ImGui-SFML options.
option(IMGUI_SFML_USE_DEFAULT_CONFIG on)
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" off)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" off)

# Find ImGui-SFML or auto-download it.
message(STATUS "Find ImGui-SFML package")
find_package(imgui-sfml NO_MODULE)
if(NOT ${imgui-sfml_FOUND})
	message(STATUS "ImGui-SFML not found, it will be auto-downloaded in the build-tree")
	include(FetchContent)
	set(FETCHCONTENT_QUIET off)
	FetchContent_Declare(
		imgui-sfml
		GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
		GIT_TAG 2.6.x
		GIT_SHALLOW on
		GIT_PROGRESS on
		EXCLUDE_FROM_ALL
		SYSTEM
		STAMP_DIR "${${PROJECT_NAME}_BUILD_DIR}"
		DOWNLOAD_NO_PROGRESS off
		LOG_DOWNLOAD on
		LOG_UPDATE on
		LOG_PATCH on
		LOG_CONFIGURE on
		LOG_BUILD on
		LOG_INSTALL on
		LOG_TEST on
		LOG_MERGED_STDOUTERR on
		LOG_OUTPUT_ON_FAILURE on
		USES_TERMINAL_DOWNLOAD on
	)
	FetchContent_MakeAvailable(imgui-sfml)
	set(IMGUI-SFML_DIR "${imgui-sfml_SOURCE_DIR}")
else()
  message(STATUS "ImGui-SFML found")
endif()

# Link ImGui-SFML to the main binary build target.
message(STATUS "Link ImGui-SFML library to the target \"${${PROJECT_NAME}_MAIN_BIN_TARGET}\"")
target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PRIVATE
		"ImGui-SFML::ImGui-SFML"
)
message(STATUS "Import and link ImGui-SFML - done")


#---- Import and link spdlog. ----
message(STATUS "Import and link spdlog")
if(DEFINED ENV{SPDLOG_DIR}) 
	set(SPDLOG_DIR "$ENV{SPDLOG_DIR}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	set(SPDLOG_DIR "D:/Documents/Software_Libraries/spdlog/1.15.0")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(SPDLOG_DIR "/opt/spdlog/1.15.0")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
	set(SPDLOG_DIR "/opt/spdlog/1.15.0")
endif()
if(DEFINED ENV{CMAKE_PREFIX_PATH}) 
	set(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")
else()
	set(CMAKE_PREFIX_PATH "${SPDLOG_DIR}")
endif()

# Find spdlog or auto-download it.
message(STATUS "Find spdlog package")
find_package(spdlog NO_MODULE)
if(NOT ${spdlog_FOUND})
	message(STATUS "spdlog not found, it will be auto-downloaded in the build-tree")
	include(FetchContent)
	set(FETCHCONTENT_QUIET off)
	FetchContent_Declare(
		spdlog
		GIT_REPOSITORY https://github.com/gabime/spdlog.git
		GIT_TAG v1.15.0
		GIT_SHALLOW on
		GIT_PROGRESS on
		EXCLUDE_FROM_ALL
		SYSTEM
		STAMP_DIR "${${PROJECT_NAME}_BUILD_DIR}"
		DOWNLOAD_NO_PROGRESS off
		LOG_DOWNLOAD on
		LOG_UPDATE on
		LOG_PATCH on
		LOG_CONFIGURE on
		LOG_BUILD on
		LOG_INSTALL on
		LOG_TEST on
		LOG_MERGED_STDOUTERR on
		LOG_OUTPUT_ON_FAILURE on
		USES_TERMINAL_DOWNLOAD on
	)
	FetchContent_MakeAvailable(spdlog)
	set(SPDLOG_DIR "${spdlog_SOURCE_DIR}")
else()
  message(STATUS "spdlog found")
endif()

# Link spdlog to the main binary build target.
message(STATUS "Link spdlog library to the target \"${${PROJECT_NAME}_MAIN_BIN_TARGET}\"")
target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
	PRIVATE
		"spdlog::spdlog"
)
message(STATUS "Import and link spdlog - done")