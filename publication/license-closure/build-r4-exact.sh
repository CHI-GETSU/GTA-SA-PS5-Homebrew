#!/bin/sh
# R4 controller DLL recipe; preserved source variant, no AUTO2 changes.
# SPDX-License-Identifier: LGPL-2.1-or-later
set -eu
: "${CORRESPONDING_SOURCE_ROOT:?set the extracted corresponding-source companion root}"
: "${R4_WORK:?set a new empty build output directory}"
[ ! -e "$R4_WORK" ] || { echo 'Choose a new R4_WORK directory' >&2; exit 2; }
here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
mkdir -p "$R4_WORK"
tree=$(python3 "$here/restore-wine.py" r4-controller \
 "$CORRESPONDING_SOURCE_ROOT/upstream/wine-490f6d5d.tar" "$R4_WORK/source")
mkdir "$R4_WORK/build"
cd "$R4_WORK/build"
"$tree/configure" --prefix=/usr --enable-archs=i386,x86_64 --disable-tests
make -j"${JOBS:-2}" dlls/dinput8/i386-windows/dinput8.dll \
 dlls/xinput1_1/i386-windows/xinput1_1.dll dlls/xinput1_2/i386-windows/xinput1_2.dll \
 dlls/xinput1_3/i386-windows/xinput1_3.dll dlls/xinput1_4/i386-windows/xinput1_4.dll \
 dlls/xinputuap/i386-windows/xinputuap.dll
