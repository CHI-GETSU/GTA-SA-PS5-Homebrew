#!/usr/bin/env bash
set -euo pipefail
PROJECT_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
export PS5_NATIVE_APP_TEMPLATE=${PS5_NATIVE_APP_TEMPLATE:-$PROJECT_ROOT/sahin/launcher}
export PS5_PAYLOAD_SDK=${PS5_PAYLOAD_SDK:-$PROJECT_ROOT/sahin/launcher/.deps/native/ps5-payload-sdk}
cd "$PROJECT_ROOT/ps5-opengl"
make test-compiler
bash toolchain/install-ps5-opengl-gl46.sh build/sdk/ps5-opengl-gl46
python3 tests/ps5/verify_gl46_link_surface.py
python3 tests/ps5/verify_gl33_capability_audit.py
