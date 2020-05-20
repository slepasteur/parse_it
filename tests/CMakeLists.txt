cmake_minimum_required(VERSION 3.8)

add_executable(parse_it_tests
    test_main.cpp
    parser/any_byte_tests.cpp
    parser/one_byte_tests.cpp
    parser/byte_seq_tests.cpp
    parser/skip_tests.cpp
    parser/integral_tests.cpp
    parser/combine_tests.cpp
    parser/or_tests.cpp
    byte_view_tests.cpp
    byte_span_tests.cpp
)

find_package(doctest MODULE REQUIRED)

target_link_libraries(parse_it_tests
  PRIVATE
    parse_it_warnings
    parse_it::parse_it
    doctest::doctest
)

set_target_properties(parse_it_tests PROPERTIES CXX_EXTENSIONS OFF)

add_test(NAME parse_it_tests COMMAND parse_it_tests)
