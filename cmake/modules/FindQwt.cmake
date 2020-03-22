# The module defines the following variables:
#  QWT_FOUND - the system has Qwt
#  QWT_INCLUDE_DIR - where to find qwt_plot.h
#  QWT_INCLUDE_DIRS - qwt includes
#  QWT_LIBRARY - where to find the Qwt library
#  QWT_LIBRARIES - aditional libraries
#  QWT_DEFINITIONS - aditional definitions
#  QWT_MAJOR_VERSION - major version
#  QWT_MINOR_VERSION - minor version
#  QWT_PATCH_VERSION - patch version
#  QWT_VERSION_STRING - version (ex. 5.2.1)
#  QWT_ROOT_DIR - root dir (ex. /usr/local)


# define the list of search paths for headers and libraries
set(FIND_QWT_PATHS
    ${QWT_ROOT}
    $ENV{QWT_ROOT}
	$ENV{PATH}
)

# find header and libraries
find_path(QWT_INCLUDE_DIR
	NAMES qwt_plot.h
	PATHS ${FIND_QWT_PATHS}
	PATH_SUFFIXES include
)
set ( QWT_INCLUDE_DIRS ${QWT_INCLUDE_DIR} )

find_library(QWT_LIBRARY
	NAMES qwt
	PATHS ${FIND_QWT_PATHS}
	PATH_SUFFIXES lib
)
find_library(QWT_LIBRARY_DEBUG
	NAMES qwtd
	PATHS ${FIND_QWT_PATHS}
	PATH_SUFFIXES lib
)
set(QWT_LIBRARIES optimized ${QWT_LIBRARY} debug ${QWT_LIBRARY_DEBUG})
set(QWT_DEFINITIONS "-DQWT_DLL")

# version
set ( _VERSION_FILE ${QWT_INCLUDE_DIR}/qwt_global.h )
if ( EXISTS ${_VERSION_FILE} )
	file ( STRINGS ${_VERSION_FILE} _VERSION_LINE REGEX "define[ ]+QWT_VERSION_STR" )
	if ( _VERSION_LINE )
		string ( REGEX REPLACE ".*define[ ]+QWT_VERSION_STR[ ]+\"(.*)\".*" "\\1" QWT_VERSION_STRING "${_VERSION_LINE}" )
		string ( REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" QWT_MAJOR_VERSION "${QWT_VERSION_STRING}" )
		string ( REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" QWT_MINOR_VERSION "${QWT_VERSION_STRING}" )
		string ( REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" QWT_PATCH_VERSION "${QWT_VERSION_STRING}" )
	endif ()
endif ()

# check version
set ( _QWT_VERSION_MATCH TRUE )
if ( Qwt_FIND_VERSION AND QWT_VERSION_STRING )
	if ( Qwt_FIND_VERSION_EXACT )
		if ( NOT Qwt_FIND_VERSION VERSION_EQUAL QWT_VERSION_STRING )
			set ( _QWT_VERSION_MATCH FALSE )
		endif ()
	else ()
		if ( QWT_VERSION_STRING VERSION_LESS Qwt_FIND_VERSION )
			set ( _QWT_VERSION_MATCH FALSE )
		endif ()
	endif ()
endif ()

# try to guess root dir from include dir
if ( QWT_INCLUDE_DIR )
	string ( REGEX REPLACE "(.*)/include.*" "\\1" QWT_ROOT_DIR ${QWT_INCLUDE_DIR} )
# try to guess root dir from library dir
elseif ( QWT_LIBRARY )
	string ( REGEX REPLACE "(.*)/lib[/|32|64].*" "\\1" QWT_ROOT_DIR ${QWT_LIBRARY} )
endif ()

# handle the QUIETLY and REQUIRED arguments
include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Qwt DEFAULT_MSG QWT_LIBRARY QWT_LIBRARIES QWT_DEFINITIONS QWT_INCLUDE_DIR QWT_INCLUDE_DIRS QWT_MAJOR_VERSION QWT_MINOR_VERSION QWT_PATCH_VERSION QWT_VERSION_STRING QWT_ROOT_DIR _QWT_VERSION_MATCH )
mark_as_advanced (
	QWT_LIBRARY
	QWT_LIBRARIES
	QWT_DEFINITIONS
	QWT_INCLUDE_DIR
	QWT_INCLUDE_DIRS
	QWT_MAJOR_VERSION
	QWT_MINOR_VERSION
	QWT_PATCH_VERSION
	QWT_VERSION_STRING
	QWT_ROOT_DIR
	_QWT_VERSION_MATCH
)