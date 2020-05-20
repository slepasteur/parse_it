cmake_minimum_required(VERSION 3.8)
project(parse_it)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(parse_it_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(parse_it_warnings)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(parse_it_options INTERFACE)
target_compile_features(parse_it_options INTERFACE cxx_std_20)

# Sanitizer options if supported by compiler.
# Enable by using -DSANITIZE_ADDRESS=ON / -DSANITIZE_MEMORY=ON
#  / -DSANITIZE_UNDEFINED_BEHAVIOR=ON / -DSANITIZE_THREAD=ON
include(cmake/Sanitizers.cmake)
enable_sanitizers(parse_it_options)


add_library(parse_it INTERFACE)
add_library(parse_it::parse_it ALIAS parse_it)

target_include_directories(parse_it INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(parse_it
  INTERFACE
    parse_it_options
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

