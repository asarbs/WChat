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
if(TARGET cpp_logger_lib)
    target_include_directories(cpp_logger_lib PUBLIC ${cpp_logger_SOURCE_DIR})
endif()

FetchContent_Declare(cpp_arguments
    GIT_REPOSITORY https://github.com/asarbs/cpp_arguments.git
    GIT_TAG master
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(cpp_arguments)
if(TARGET cpp_arguments_lib)
    target_include_directories(cpp_arguments_lib PUBLIC ${cpp_arguments_SOURCE_DIR})
endif()

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

FetchContent_Declare(cpp_config
    GIT_REPOSITORY https://github.com/asarbs/cpp_config.git
    GIT_TAG master
    GIT_SHALLOW OFF
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(cpp_config)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system)

message(STATUS "Application name: ${TARGET_NAME_CLIENT}")

add_executable(${TARGET_NAME_CLIENT}
    ChatClient.cpp
    client.cpp
    internal/CommandsExcutor/RegisterSession.cpp
    internal/CommandsExcutor/SendMsg.cpp
    internal/CommandsExcutor/Contacts.cpp
    internal/CUI.cpp
    internal/Message.cpp
    proto/messages.pb.cc
    server/api/ProtoDecoder.cpp
    server/api/ProtoWrapper.cpp
    server/connection/MessagesReceiver.cpp
    server/connection/ThreadSafeQueue.cpp
    server/connection/WebSocketWorker.cpp
)

target_compile_definitions(${TARGET_NAME_CLIENT}
    PRIVATE
        TARGET_NAME_CLIENT="${TARGET_NAME_CLIENT}"
)

target_link_libraries(${TARGET_NAME_CLIENT}
    PRIVATE
        Boost::system
        pthread
        crypto
        nlohmann_json::nlohmann_json
        cpp_logger_lib
        cpp_arguments_lib
        cpp_config
        protobuf
        ixwebsocket
    )

target_include_directories(${TARGET_NAME_CLIENT}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/client/cpp
)
