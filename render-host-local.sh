#!/bin/bash
set -euo pipefail
PROJECT_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
export CPATH=${PROJECT_ROOT}/sahin/launcher/.deps/pacbrew/v0.40.2/sysroot/user/homebrew/include:${PROJECT_ROOT}/ps5-opengl/build/sdk/ps5-opengl-gl46/include
export LIBRARY_PATH=${PROJECT_ROOT}/host-libs
export BUILD_JOBS=4 USE_CCACHE=0 STORE_APP=dist/PPSA99604
cd ${PROJECT_ROOT}/sahin/launcher
bash tools/store-host.sh
