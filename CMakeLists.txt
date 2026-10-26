cmake_minimum_required(VERSION 3.20)
project(FidelKey LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

option(BUILD_TESTS "Build the engine test suite" ON)

if(BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(engine)

if(WIN32)
    add_subdirectory(tsf-service)
endif()
