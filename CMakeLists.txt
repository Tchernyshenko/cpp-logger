cmake_minimum_required(VERSION 3.14)
project(cpp_logger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only библиотека
add_library(cpp_logger INTERFACE)

target_include_directories(cpp_logger INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Опции сборки
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)

if(BUILD_EXAMPLES)
    add_executable(logger_example examples/basic_usage.cpp)
    target_link_libraries(logger_example PRIVATE cpp_logger)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_executable(logger_tests tests/logger_test.cpp)
    target_link_libraries(logger_tests PRIVATE cpp_logger)
    add_test(NAME LoggerTests COMMAND logger_tests)
endif()
