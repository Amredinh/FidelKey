# Build FidelKey TSF Service

cmake_minimum_required(VERSION 3.10)
project(FidelKeyTSF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build engine as static library (needed for TSF)
add_subdirectory(../engine engine-build EXCLUDE_FROM_ALL)

add_library(FidelKeyTSF SHARED
    src/dllmain.cpp
    src/TextService.cpp
    src/Registration.cpp
)

target_include_directories(FidelKeyTSF
    PUBLIC
        include
        ../engine/include
)

target_link_libraries(FidelKeyTSF
    PRIVATE
        msctf
        ole32
        uuid
        fidelkey_engine
)

# For Windows
if(WIN32)
    set_target_properties(FidelKeyTSF PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        PREFIX ""
    )
endif()
