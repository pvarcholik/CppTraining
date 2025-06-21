#!/usr/bin/env bash

set -e

target="$1"

# Mapping from target to build subdirectory
case "$target" in
  cpp_training_common_tests)
    test_subdir="src/common/tests"
    ;;
  cpp_training_lesson_0_tests)
    test_subdir="src/lesson_0/tests"
    ;;
  *)
    echo "❌ Unknown test target: $target"
    exit 1
    ;;
esac

echo "ℹ️  Running test: ./build/${test_subdir}/${target}"

mkdir -p build/coverage
lcov --directory build --zerocounters

./build/${test_subdir}/${target}

lcov --capture --directory build --output-file build/coverage/${target}.info
lcov --remove build/coverage/${target}.info "/usr/include/*" "*/vcpkg/*" \
  --output-file build/coverage/${target}.filtered.info
genhtml build/coverage/${target}.filtered.info \
  --output-directory build/coverage/${target}_html

echo "✅ Coverage report at: build/coverage/${target}_html/index.html"