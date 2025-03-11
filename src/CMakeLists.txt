# World VTT
#
# Copyright (C) 2024, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.


set(TARGET_NAME_SERVER ${PROJECT_NAME}_SERVER)
set(TARGET_NAME_CLIENT ${PROJECT_NAME}_CLIENT)

FetchContent_Declare(cpp_logger
    GIT_REPOSITORY https://github.com/asarbs/cpp_logger.git
    GIT_TAG main
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(cpp_logger)

FetchContent_Declare(cpp_arguments
    GIT_REPOSITORY https://github.com/asarbs/cpp_arguments.git
    GIT_TAG master
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(cpp_arguments)

FetchContent_Declare(yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG master
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(yaml-cpp)

FetchContent_Declare(websocketpp
    GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
    GIT_TAG 0.8.2
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(websocketpp)

FetchContent_Declare(FTXUI
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v5.0.0
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(FTXUI)

find_package(Boost REQUIRED)


message(STATUS "Application name: ${TARGET_NAME_SERVER} ${TARGET_NAME_CLIENT}")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found!")
endif()

add_executable(${TARGET_NAME_SERVER}
    server.cpp
)

target_link_libraries(${TARGET_NAME_SERVER}
    PRIVATE
        Boost::system
        pthread
        crypto
    )

target_include_directories(${TARGET_NAME_SERVER}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/build/linux/cpp_logger
        ${CMAKE_SOURCE_DIR}/build/linux/cpp_arguments
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/websocketpp-src
)


add_executable(${TARGET_NAME_CLIENT}
    client.cpp
)

target_link_libraries(${TARGET_NAME_CLIENT}
    PRIVATE
        Boost::system
        pthread
        crypto
)

target_include_directories(${TARGET_NAME_CLIENT}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/build/linux/cpp_logger
        ${CMAKE_SOURCE_DIR}/build/linux/cpp_arguments
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/websocketpp-src
)




