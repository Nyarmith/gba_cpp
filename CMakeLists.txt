cmake_minimum_required(VERSION 3.10)

project(gba_demos)

include(cmake/devKitPro.cmake)
include(cmake/submodules.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -DSF_VISIBILITY -fvisibility=hidden -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
update_submodules()
add_subdirectory(external)

include_directories(include)
add_subdirectory(demos)
