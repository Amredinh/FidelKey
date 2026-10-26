cmake_minimum_required(VERSION 3.20)
project(FidelKeyEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS ON)
endif()

# Engine library
add_library(fidelkey_engine STATIC
    src/trie.cpp
    src/mapping.cpp
    src/engine.cpp
)

target_include_directories(fidelkey_engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
