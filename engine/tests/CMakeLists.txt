cmake_minimum_required(VERSION 3.10)

add_executable(engine_tests
    test_main.cpp
)

target_link_libraries(engine_tests
    PRIVATE
        fidelkey_engine
)

target_include_directories(engine_tests
    PRIVATE
        ../include
)

# Add test
add_test(NAME selam_test COMMAND engine_tests)
