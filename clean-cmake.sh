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

readonly WORKSPACE_PATH=$(pwd)
declare -r BUILD_PATH="${WORKSPACE_PATH}/build"
declare -r SOLUTION_PATH="${BUILD_PATH}/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}-${PROJECT_VERSION_MINOR}-${PROJECT_VERSION_PATCH}-linux"

if [[ -d "${SOLUTION_PATH}" ]]; then
	cd "${SOLUTION_PATH}"
	cmake --build . --target clean
	cd "${WORKSPACE_PATH}"
	rm -r "${SOLUTION_PATH}"
fi

if [[ "${?}" -eq 0 ]]; then
	echo "The solution was successfully cleaned!"
fi

exit $?
