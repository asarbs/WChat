/*
 * World VTT
 *
 * Copyright (C) 2024, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 *
 */

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <string>
#include <websocketpp/server.hpp>

class ChatClient {
    public:
        ChatClient();
        explicit ChatClient(uint64_t user_id);
        ChatClient(const ChatClient& other);
        ChatClient& operator=(const ChatClient& other);
        ~ChatClient();
        uint64_t get_user_id() {  // cppcheck-suppress unusedFunction
            return __user_id;
        }
        websocketpp::connection_hdl connection;

    protected:
    private:
        // Remove the ability to move
        ChatClient(ChatClient&&)            = delete;
        ChatClient& operator=(ChatClient&&) = delete;

        uint64_t __user_id;
};

#endif
