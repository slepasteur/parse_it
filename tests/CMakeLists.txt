add_executable(parse_it_tests
    test_main.cpp
    parser/any_byte_tests.cpp
    parser/one_byte_tests.cpp
    parser/byte_seq_tests.cpp
    parser/skip_tests.cpp
    parser/integral_tests.cpp
)

target_link_libraries(parse_it_tests parse_it)