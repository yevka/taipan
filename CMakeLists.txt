cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

# Setup project
# We parse the canonical version number located in <include/version.h>.
# This is done to allow the library to be used without requiring a proper
# installation during which the version would be written to this header.
foreach(level MAJOR MINOR PATCH)
    file(STRINGS include/version.h
            _define_${level}
            REGEX "#define TAIPAN_${level}_VERSION")
    string(REGEX MATCH "([0-9]+)" _version_${level} "${_define_${level}}")
endforeach()

set(TAIPAN_VERSION_STRING "${_version_MAJOR}.${_version_MINOR}.${_version_PATCH}")
project(taipan
        VERSION ${TAIPAN_VERSION_STRING}
        LANGUAGES CXX)

message(STATUS "${CMAKE_PROJECT_NAME} version: " ${PROJECT_VERSION})


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} "Realese")
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

set(INSTALL_BIN_DIR_NAME bin CACHE STRING "Installation directory name for executables")
set(ENABLE_TESTING ON CACHE BOOL "Enable unit tests")

include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB_RECURSE SRC_LIST "src/*.cpp")
set(MAIN "main.cpp")

# Define a grouping for source files in Visual Studio 2015 project generation
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
file(GLOB_RECURSE HEADERS "include/*.h")
source_group("include" FILES ${HEADERS})

add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SRC_LIST} ${MAIN})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY FOLDER "executables")

install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION ${INSTALL_BIN_DIR_NAME})

if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
    message(STATUS "Unit tests status: ENABLED")
else (ENABLE_TESTING)
    message(STATUS "Unit tests status: DISABLED")
endif (ENABLE_TESTING)