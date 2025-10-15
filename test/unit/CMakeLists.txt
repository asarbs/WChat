# World VTT
#
# Copyright (C) 2024, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.

enable_testing()

file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/src/server/**/*.cpp
    ${CMAKE_SOURCE_DIR}/src/server/**/*.cc
    ${CMAKE_SOURCE_DIR}/src/server/*.cpp
)
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*/server\\.cpp$")

add_executable(unit_tests ${TEST_SOURCES})

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(unit_tests PRIVATE
    gtest
    gtest_main
    cpp_logger_lib
    cpp_arguments_lib
    nlohmann_json::nlohmann_json
    pthread
    protobuf
)

add_test(NAME unit_tests COMMAND unit_tests --gtest_catch_exceptions=0)

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/build/linux/_deps/websocketpp-src
    ${CMAKE_SOURCE_DIR}/build/linux/_deps/cpp_logger-src
    ${CMAKE_SOURCE_DIR}/build/linux/_deps/cpp_arguments-src
    ${CMAKE_SOURCE_DIR}build/linux/_deps/protobuf-src/src
)
