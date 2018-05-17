#!/bin/bash

set -ev

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
    bash ./ci/install-cmake-for-linux.sh
else
    if [[ -z "which cmake" ]]; then
        brew install cmake
    fi
fi
