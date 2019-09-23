cmake_minimum_required(VERSION 3.6)

project(
  pantomime
  DESCRIPTION "An ncurses-based MPD client."
  HOMEPAGE_URL "https://github.com/LeftySolara/pantomime"
  LANGUAGES C
  VERSION 0.1
)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2")

set(CMAKE_BUILD_TYPE Debug)

# Recursively add source files to the build tree
file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(pantomime ${SOURCE_FILES})

set_target_properties(pantomime PROPERTIES OUTPUT_NAME "pantomime")
