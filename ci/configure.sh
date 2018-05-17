#!/bin/bash

set -ev

# Run CMake
cd "${TRAVIS_BUILD_DIR}"
if [ -d "${BUILD_DIR_NAME}" ]; then
    rm -rf "${BUILD_DIR_NAME}"
fi

mkdir "${BUILD_DIR_NAME}"
cd "${BUILD_DIR_NAME}"

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
    export PATH=$HOME/cmake/bin:$PATH
fi

cmake --version

cmake "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}" ..
