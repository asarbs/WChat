/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 *
 */

#ifndef CHATCLIENTDATABASE_H
#define CHATCLIENTDATABASE_H

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <websocketpp/server.hpp>

#include "ChatClient.h"

namespace WChat::ChatServer::client {

    struct hdl_compare {
            bool operator()(const websocketpp::connection_hdl& lhs, const websocketpp::connection_hdl& rhs) const {
                return lhs.owner_before(rhs);  // porównuje weak_ptr-y
            }
    };

    class ChatClientDatabase {
        public:
            static ChatClientDatabase& getInstance();
            uint64_t regiserClinet(websocketpp::connection_hdl hdl, const std::string& new_user_name);
            uint64_t regiserClinet(uint64_t user_id);
            uint64_t getUserIdByName(const std::string& name);
            bool unregiserClinet(uint64_t user_id);
            std::shared_ptr<ChatClient> get(uint64_t user_id);
            void clean();

            size_t size() const {
                return __chat_clients.size();
            }

            ChatClientDatabase(const ChatClientDatabase&)            = delete;
            ChatClientDatabase& operator=(const ChatClientDatabase&) = delete;

        protected:
        private:
            ChatClientDatabase();
            ~ChatClientDatabase() = default;
            std::map<uint64_t, std::shared_ptr<ChatClient>> __chat_clients{};
            uint64_t __next_free_user_id;
    };
};  // namespace WChat::ChatServer::client
#endif
