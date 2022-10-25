cmake_minimum_required(VERSION 3.12)

project(
  Pantomime
  DESCRIPTION "An NCURSES MPD client."
  HOMEPAGE_URL "https://git.julianneadams.info/LeftySolara/pantomime"
  LANGUAGES C
  VERSION 0.2.0
)

set(PROJECT Pantomime)

configure_file(${CMAKE_SOURCE_DIR}/cmake/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Wformat=2 -Wunused-parameter -Wshadow -Wstrict-prototypes -Wredundant-decls -Wnested-externs -Wmissing-include-dirs")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2")

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories (${CMAKE_CURRENT_BINARY_DIR})

# NCURSES stuff
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_subdirectory(src)

# Recursively add source files to the build tree
file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(Pantomime ${SOURCE_FILES})

set_target_properties(Pantomime PROPERTIES OUTPUT_NAME "pantomime")
target_link_libraries(Pantomime -lpanel ${CURSES_LIBRARIES})
target_link_libraries(Pantomime mpdclient)

target_include_directories(Pantomime PUBLIC "${PROJECT_BINARY_DIR}")