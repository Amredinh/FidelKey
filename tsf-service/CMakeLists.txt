# Build FidelKey TSF Service

cmake_minimum_required(VERSION 3.20)
project(FidelKeyTSF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Allow both a standalone build of this directory and inclusion from the root.
if(NOT TARGET fidelkey_engine)
    add_subdirectory(../engine engine-build EXCLUDE_FROM_ALL)
endif()

add_library(FidelKeyTSF SHARED
    src/dllmain.cpp
    src/TextService.cpp
    src/Registration.cpp
    FidelKeyTSF.def
)

target_include_directories(FidelKeyTSF
    PUBLIC
        include
        ../engine/include
)

target_compile_definitions(FidelKeyTSF
    PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _WIN32_WINNT=0x0A00
)

target_link_libraries(FidelKeyTSF
    PRIVATE
        fidelkey_engine
        msctf
        ole32
        oleaut32
        uuid
        advapi32
)

if(WIN32)
    set_target_properties(FidelKeyTSF PROPERTIES
        OUTPUT_NAME "FidelKeyTSF"
        PREFIX ""
    )
endif()
