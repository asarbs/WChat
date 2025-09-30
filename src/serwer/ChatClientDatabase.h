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

#ifndef CHATCLIENTDATABASE_H
#define CHATCLIENTDATABASE_H

#pragma once

#include <vector>
#include <websocketpp/server.hpp>

#include "ChatClient.h"

struct hdl_compare {
        bool operator()(const websocketpp::connection_hdl& lhs, const websocketpp::connection_hdl& rhs) const {
            return lhs.owner_before(rhs);  // porównuje weak_ptr-y
        }
};

class ChatClientDatabase {
    public:
        static ChatClientDatabase& getInstance();
        void regiserClinet(websocketpp::connection_hdl hdl);
        size_t size() const {
            return __chat_clients.size();
        }

        ChatClientDatabase(const ChatClientDatabase&)            = delete;
        ChatClientDatabase& operator=(const ChatClientDatabase&) = delete;

    protected:
    private:
        ChatClientDatabase()  = default;
        ~ChatClientDatabase() = default;
        std::map<websocketpp::connection_hdl, ChatClient, hdl_compare> __chat_clients{};
};

#endif
