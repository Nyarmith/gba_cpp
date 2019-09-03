cmake_minimum_required(VERSION 3.10)

project(gba_demos)

include(cmake/devKitPro.cmake)
include(cmake/submodules.cmake)
update_submodules()

# build git modules
add_subdirectory(external)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs not available" )

add_gba_target(snake bmp_mode_snake.cc)
