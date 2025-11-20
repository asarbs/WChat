# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.


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

FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG v11.4.6
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ixwebsocket)

FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(gtest)

FetchContent_Declare(protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG v33.0-rc1
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(protobuf)

find_package(Boost REQUIRED)

message(STATUS "Application name: ${TARGET_NAME_CLIENT}")

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

add_executable(${TARGET_NAME_CLIENT}
    ChatClient.cpp
    client.cpp
    internal/Message.cpp
    proto/messeges.pb.cc
    ServerApi/ProtoWrapper.cpp
    ServerApi/ProtoDecoder.cpp
    ThreadSafeQueue.cpp
    WebSocketWorker.cpp
    internal/CUI.cpp
    internal/CommandsExcutor.cpp
)

target_link_libraries(${TARGET_NAME_CLIENT}
    PRIVATE
        Boost::system
        pthread
        crypto
        nlohmann_json::nlohmann_json
        cpp_logger_lib
        cpp_arguments_lib
        protobuf
        ixwebsocket
    )

target_include_directories(${TARGET_NAME_CLIENT}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/client/cpp
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/cpp_logger-src
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/cpp_arguments-src
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/websocketpp-src
        ${CMAKE_SOURCE_DIR}build/linux/_deps/protobuf-src/src
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/sqlight-src/include/
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/ftxui-src/include
        ${CMAKE_SOURCE_DIR}/build/linux/_deps/imtui-src/include
)

