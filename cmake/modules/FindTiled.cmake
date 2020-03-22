# The module defines the following variables:
#  TILED_LIBRARY
#  TILED_LIBRARIES
#  TILED_DEFINITIONS
#  TILED_INCLUDE_DIR
#  TILED_INCLUDE_DIRS
#  TILED_ROOT_DIR
	
# define the list of search paths for headers and libraries
set(FIND_TILED_PATHS
	${TILED_ROOT}
	$ENV{TILED_ROOT}
	$ENV{PATH}
)

# find header and libraries
find_path(TILED_INCLUDE_DIR
	NAMES tiled.h
	PATHS ${FIND_TILED_PATHS}
	PATH_SUFFIXES include
)
set ( TILED_INCLUDE_DIRS ${TILED_INCLUDE_DIR} )

find_library(TILED_LIBRARY
	NAMES tiled
	PATHS ${FIND_TILED_PATHS}
	PATH_SUFFIXES lib
)
find_library(TILED_LIBRARY_DEBUG
	NAMES tiledd
	PATHS ${FIND_TILED_PATHS}
	PATH_SUFFIXES lib
)
set(TILED_LIBRARIES optimized ${TILED_LIBRARY} debug ${TILED_LIBRARY_DEBUG})
set(TILED_DEFINITIONS "-DTILED_DLL")

# try to guess root dir from include dir
if ( TILED_INCLUDE_DIR )
	string ( REGEX REPLACE "(.*)/include.*" "\\1" TILED_ROOT_DIR ${TILED_INCLUDE_DIR} )
# try to guess root dir from library dir
elseif ( TILED_LIBRARY )
	string ( REGEX REPLACE "(.*)/lib[/|32|64].*" "\\1" TILED_ROOT_DIR ${TILED_LIBRARY} )
endif ()

# handle the QUIETLY and REQUIRED arguments
include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Tiled DEFAULT_MSG TILED_LIBRARY TILED_LIBRARIES TILED_DEFINITIONS TILED_INCLUDE_DIR TILED_INCLUDE_DIRS TILED_ROOT_DIR)
mark_as_advanced (
	TILED_LIBRARY
	TILED_LIBRARIES
	TILED_DEFINITIONS
	TILED_INCLUDE_DIR
	TILED_INCLUDE_DIRS
	TILED_ROOT_DIR
)