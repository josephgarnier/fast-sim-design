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
	RELEASE_NAME "tiled_1.11.0"
	DEBUG_NAME "tiledd_1.11.0"
	ROOT_DIR "${${PROJECT_NAME}_LIB_DIR}"
	INCLUDE_DIR "${${PROJECT_NAME}_INCLUDE_DIR}/tiled"
)
list(APPEND ${PROJECT_NAME}_IMPORTED_INTERNAL_LIBRARIES "tiled")
