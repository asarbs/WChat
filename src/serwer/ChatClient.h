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

#include <list>
#include <string>
#include <websocketpp/server.hpp>

class ChatClient {
    public:
        struct MsgHolder {
                uint64_t from;
                std::string message;
        };

        ChatClient();
        explicit ChatClient(uint64_t user_id, const std::string& name);
        ChatClient(const ChatClient& other);
        ~ChatClient();
        ChatClient& operator=(const ChatClient& other);

        void registerClient();
        void unregister();
        void saveMsg(uint64_t from, std::string message);
        bool isRegistered() const;
        uint64_t getUserId() const;
        const std::string& getName() const;
        bool hasMsg() const;
        MsgHolder popMsg();

        websocketpp::connection_hdl connection;

    protected:
        //
    private:
        // Remove the ability to move
        ChatClient(ChatClient&&)            = delete;
        ChatClient& operator=(ChatClient&&) = delete;

        uint64_t __user_id;
        std::string __name;
        bool __is_registered;
        std::list<MsgHolder> __savedMsg = {};
};

#endif
