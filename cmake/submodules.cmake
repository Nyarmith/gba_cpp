cmake_minimum_required(VERSION 3.10)

function(update_submodules)
    set(DEST ${PROJECT_SOURCE_DIR})

    execute_process(
        COMMAND git submodule update --init --recursive
        WORKING_DIRECTORY "${DEST}"
        RESULT_VARIABLE result)

    if (NOT ${result} EQUAL 0)
        message(FATAL_ERROR "unable to update submodules")
    endif()
endfunction()
