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
#include "server/core/storage/StorageFactory.h"
#include "server/errors/ClientException.h"

namespace WChat::ChatServer::client {

    ChatClientDatabase& ChatClientDatabase::getInstance() {
        static ChatClientDatabase instance;
        return instance;
    }

    ChatClientDatabase::ChatClientDatabase() : _storage(WChat::ChatServer::core::storage::getStorage()) {
    }

    // ChatClientDatabase::~ChatClientDatabase() {
    // }

    uint64_t ChatClientDatabase::regiserClinetSession(websocketpp::connection_hdl hdl, const std::string& new_user_name) {
        uint64_t uid = getUserIdByName(new_user_name);
        if (uid != UINT64_MAX) {
            return uid;
        }

        std::optional<uint64_t> userId = _storage->getUserIdByName(new_user_name);
        if (!userId) {
            throw WChat::ChatServer::errors::client::ClientNotRegistered(new_user_name);
        }
        std::shared_ptr<ChatClient> cc = std::make_shared<ChatClient>(*userId, new_user_name);
        cc->connection                 = hdl;
        cc->registerClient();
        _chat_clients[*userId] = cc;
        logger::logger << logger::debug << "ChatClientDatabase::regiserClinetSession with id = " << *userId << "." << logger::endl;
        return *userId;
    }

    bool ChatClientDatabase::unregiserClinet(uint64_t user_id) {
        std::shared_ptr<ChatClient> cc = get(user_id);
        cc->unregister();
        return true;
    }

    std::shared_ptr<ChatClient> ChatClientDatabase::get(uint64_t user_id) {
        auto it = _chat_clients.find(user_id);
        if (it != _chat_clients.end()) {
            return it->second;
        }
        return nullptr;
    }
    void ChatClientDatabase::clean() {  // cppcheck-suppress unusedFunction
        _chat_clients.clear();
    }

    uint64_t ChatClientDatabase::getUserIdByName(const std::string& uname) {
        for (auto cc : _chat_clients) {
            if (cc.second->getName() == uname) {
                return cc.first;
            }
        }
        return UINT64_MAX;
    }
};  // namespace WChat::ChatServer::client
