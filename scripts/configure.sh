#!/usr/bin/env bash

set -e

# Try to use VCPKG_ROOT or a default fallback
VCPKG_TOOLCHAIN_FILE="${VCPKG_ROOT:-$HOME/dev/vcpkg}/scripts/buildsystems/vcpkg.cmake"

mkdir -p build
cd build

cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN_FILE"