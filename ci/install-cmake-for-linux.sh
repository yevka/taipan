#!/bin/bash

set -ev
CMAKE_VERSION_MAJOR=3.7
CMAKE_VERSION="${CMAKE_VERSION_MAJOR}.0"

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
    if [ ! -d "$HOME/cmake/bin" ] || [[ "`$HOME/cmake/bin/make --version`" != *"${CMAKE_VERSION}"* ]]; then
        curl -L -O http://www.cmake.org/files/v${CMAKE_VERSION_MAJOR}/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz
        tar -xzf cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz
        cp -fa cmake-${CMAKE_VERSION}-Linux-x86_64/. $HOME/cmake/
    fi
fi
