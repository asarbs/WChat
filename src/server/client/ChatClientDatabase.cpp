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

#include "ChatClientDatabase.h"

#include "logger.h"

namespace WChat::ChatServer::client {

    ChatClientDatabase& ChatClientDatabase::getInstance() {
        static ChatClientDatabase instance;
        return instance;
    }

    ChatClientDatabase::ChatClientDatabase() : __next_free_user_id(0) {
    }

    // ChatClientDatabase::~ChatClientDatabase() {
    // }

    uint64_t ChatClientDatabase::regiserClinet(websocketpp::connection_hdl hdl, const std::string& new_user_name) {
        uint64_t uid = getUserIdByName(new_user_name);
        if (uid != UINT64_MAX) {
            return uid;
        }

        std::shared_ptr<ChatClient> cc = std::make_shared<ChatClient>(__next_free_user_id, new_user_name);
        cc->connection                 = hdl;
        cc->registerClient();
        __chat_clients[__next_free_user_id] = cc;
        uint64_t current_client_id          = __next_free_user_id;
        logger::logger << logger::debug << "ChatClientDatabase::regiserClinet with id = " << __next_free_user_id << "." << logger::endl;
        __next_free_user_id++;
        return current_client_id;
    }

    bool ChatClientDatabase::unregiserClinet(uint64_t user_id) {
        std::shared_ptr<ChatClient> cc = get(user_id);
        cc->unregister();
        return true;
    }

    std::shared_ptr<ChatClient> ChatClientDatabase::get(uint64_t user_id) {
        auto it = __chat_clients.find(user_id);
        if (it != __chat_clients.end()) {
            return it->second;
        }
        return nullptr;
    }
    void ChatClientDatabase::clean() {  // cppcheck-suppress unusedFunction
        __chat_clients.clear();
        __next_free_user_id = 0;
    }

    uint64_t ChatClientDatabase::getUserIdByName(const std::string& uname) {
        for (auto cc : __chat_clients) {
            if (cc.second->getName() == uname) {
                return cc.first;
            }
        }
        return UINT64_MAX;
    }
};  // namespace WChat::ChatServer::client
