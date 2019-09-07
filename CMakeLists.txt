cmake_minimum_required(VERSION 3.10)

project(gba_demos)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/devKitPro.cmake)
include(cmake/submodules.cmake)

update_submodules()
add_subdirectory(external)

include_directories(include)
add_subdirectory(demos)
