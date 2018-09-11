cmake_minimum_required(VERSION 3.8)
project(parse_it)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wno-missing-braces -fno-omit-frame-pointer -fsanitize=address")


add_library(parse_it INTERFACE)
target_include_directories(parse_it INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/parse_it>
    $<INSTALL_INTERFACE:include/parse_it>
)

add_subdirectory(tests)
enable_testing()
add_test(NAME ParseItUnitTest COMMAND parse_it_tests)