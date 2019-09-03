cmake_minimum_required(VERSION 3.10)
# https://github.com/sebknzl/krawall/blob/master/cmake/FindGba.cmake


set(CMAKE_SYSTEM_NAME Generic)

find_program(CMAKE_C_COMPILER arm-none-eabi-gcc)
if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "arm-none-eabi-gcc not found, check your devKitPro environment")
endif()

find_program(CMAKE_CXX_COMPILER arm-none-eabi-gcc)
if(NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "arm-none-eabi-g++ not found, check your devKitPro environment")
endif()

find_program(OBJCOPY_EXE arm-none-eabi-objcopy)
if(NOT OBJCOPY_EXE)
    message(FATAL_ERROR "arm-none-eabi-objcopy not found, check your devKitPro environment")
endif()

# overriding CMAKE_OBJCOPY doesn't work for some reason
find_program(OBJCOPY_EXE arm-none-eabi-objcopy)
if(NOT OBJCOPY_EXE)
    message(FATAL_ERROR "arm-none-eabi-objcopy not found, check your devKitPro environment")
endif()

find_program(GBAFIX_EXE gbafix)
if(NOT GBAFIX_EXE)
    message(FATAL_ERROR "gbafix not found, check your devKitPro environment")
endif()

set(PKG_CONFIG_EXECUTABLE "arm-none-eabi-pkg-config")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs not available" )


function(add_gba_target target_ sources_)
    add_executable(${target_} ${sources_})
    set_target_properties(${target_} PROPERTIES OUTPUT_NAME "${target_}.elf")
    target_compile_options(${target_} PRIVATE -mthumb-interwork -mthumb -O2)
    target_link_options(${target_} PRIVATE -mthumb-interwork -mthumb -specs=gba.specs)
    add_custom_command(TARGET ${target_} POST_BUILD
        COMMAND ${OBJCOPY_EXE} -v -O binary ${target_}.elf ${target_}.gba
        COMMAND ${GBAFIX_EXE} ${target_}.gba)
endfunction()
