# Copyright 2019-present, Joseph Garnier
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.

#!/bin/bash

export PROJECT_NAME="fast-sim-design"
export PROJECT_VERSION_MAJOR="0"
export PROJECT_VERSION_MINOR="1"
export PROJECT_VERSION_PATCH="0"

readonly WORKSPACE_DIR=$(pwd)
declare -r BUILD_DIR="${WORKSPACE_DIR}/build"
declare -r SOLUTION_DIR="${BUILD_DIR}/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}-${PROJECT_VERSION_MINOR}-${PROJECT_VERSION_PATCH}-linux"

if [[ -d "${SOLUTION_DIR}" ]]; then
	cmake --build "${SOLUTION_DIR}" --target clean

	# Remove solution in build directory
	rm -rf "${SOLUTION_DIR}"
fi

if [[ "${?}" -eq 0 ]]; then
	echo "The solution was successfully cleaned!"
fi

exit $?
