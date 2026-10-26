cmake_minimum_required(VERSION 3.10)
project(FidelKeyEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
