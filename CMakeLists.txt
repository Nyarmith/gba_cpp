cmake_minimum_required(VERSION 3.10)

project(gba_demos)

include(cmake/devKitPro.cmake)
include(cmake/submodules.cmake)

update_submodules()

add_subdirectory(external)
add_subdirectory(demos)
