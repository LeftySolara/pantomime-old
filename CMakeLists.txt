cmake_minimum_required(VERSION 3.6)
project(Pantomime)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -O0")
set(CMAKE_BUILD_TYPE Debug)

set(Pantomime_VERSION_MAJOR 0)
set(Pantomime_VERSION_MINOR 1)

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/PantomimeConfig.h.in"
  "${PROJECT_BINARY_DIR}/PantomimeConfig.h"
  )

# Add the binary tree to the search path for include files so we'll find PantomimeConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# Recursively add source files to the build tree
file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(Pantomime ${SOURCE_FILES})
set_target_properties(Pantomime PROPERTIES OUTPUT_NAME "pantomime")
