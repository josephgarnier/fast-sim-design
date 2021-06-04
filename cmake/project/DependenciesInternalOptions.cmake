# Copyright 2019-present, Joseph Garnier
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.
# =============================================================================
# What Is This?
# -------------
# See README file in the root directory of this source tree.

include(Dependency)

# Note: set the list of names of imported libraries coming from `lib/` directory.
set(${PROJECT_NAME}_IMPORTED_INTERNAL_LIBRARIES "")

#------------------------------------------------------------------------------
# Import internal libraries from here.
#------------------------------------------------------------------------------

#---- Import Tiled. ----
dependency(IMPORT "tiled"
	SHARED
	DEBUG_NAME "tiledd"
	ROOT_DIR "${${PROJECT_NAME}_LIB_DIR}"
	INCLUDE_DIR "${${PROJECT_NAME}_INCLUDE_DIR}/tiled"
)
list(APPEND ${PROJECT_NAME}_IMPORTED_INTERNAL_LIBRARIES "tiled")

#---- Import zlib. ----
dependency(IMPORT "zlib"
	SHARED
	RELEASE_NAME "zlib"
	DEBUG_NAME "zlib"
	ROOT_DIR "${${PROJECT_NAME}_LIB_DIR}"
	INCLUDE_DIR "${${PROJECT_NAME}_INCLUDE_DIR}/zlib"
)
list(APPEND ${PROJECT_NAME}_IMPORTED_INTERNAL_LIBRARIES "zlib")
