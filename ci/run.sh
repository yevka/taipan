#!/bin/bash

set -ev

cd "${TRAVIS_BUILD_DIR}/${BUILD_DIR_NAME}"
make -j2 VERBOSE=1
