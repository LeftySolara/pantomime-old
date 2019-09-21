cmake_minimum_required(VERSION 3.6)
project(pantomime)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -O0")
set(CMAKE_BUILD_TYPE Debug)

set(PANTOMIME_VERSION_MAJOR 0)
set(PANTOMIME_VERSION_MINOR 1)

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/pantomimeConfig.h.in"
  "${PROJECT_BINARY_DIR}/pantomimeConfig.h"
)

# Add the binary tree to the search path for include files so we'll find pantomimeConfig.h
include_directories("$PROJECT_BINARY_DIR")

# Recursively add source files to the build tree
file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(pantomime ${SOURCE_FILES})

set_target_properties(pantomime PROPERTIES OUTPUT_NAME "pantomime")
