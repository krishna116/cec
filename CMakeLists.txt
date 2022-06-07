cmake_minimum_required(VERSION 3.15)

## cec is cute enum compiler.
project(cec VERSION 1.0.0 LANGUAGES CXX)

##-------------------------------------------------------------
option(ENABLE_CLANG_TIDY "enable clang-tidy test" FALSE)
option(ENABLE_UNIT_TEST "enable unit test" TRUE)
set(CMAKE_CXX_STANDARD 11)

##-------------------------------------------------------------
if(ENABLE_CLANG_TIDY)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY 
            "${CLANG_TIDY_EXE}" 
            CACHE STRING "" FORCE)
    else()
        message(STATUS "clang-tidy not found!")
        set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
    endif()
endif()

##-------------------------------------------------------------
configure_file(config.h.txt config.h)
add_subdirectory(src)

##-------------------------------------------------------------
if(${ENABLE_UNIT_TEST})
    enable_testing()
    add_subdirectory(test)
endif()
