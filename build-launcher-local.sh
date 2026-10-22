#!/usr/bin/env bash
set -euo pipefail
PROJECT_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
export PROJECT_ROOT
export CXX=${CXX:-/usr/bin/clang++}
export PS5_OPENGL_PREFIX=${PS5_OPENGL_PREFIX:-$PROJECT_ROOT/ps5-opengl/build/sdk/ps5-opengl-gl46}
export PS5_ASSETS_TOOL=${PS5_ASSETS_TOOL:-$PROJECT_ROOT/assets-tools/tools/prepare-assets.sh}
export TMPDIR=${TMPDIR:-$PROJECT_ROOT/build/tmp}
mkdir -p "$TMPDIR"
make -C "$PROJECT_ROOT/sahin/launcher" -o blackbox-assets -o store-worker -o self-update-helper -o runtime/libc.prx GAME=PPSA99604 USE_CCACHE=0 BUILD_JOBS="${BUILD_JOBS:-4}" PS5_CLANG="${PS5_CLANG:-clang-18}" app
