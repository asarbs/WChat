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

    void ChatClientDatabase::clean() {
        _storage->clean();
    }

    uint64_t ChatClientDatabase::regiserClinetSession(websocketpp::connection_hdl hdl, const std::string& new_user_name) {
        uint64_t uid = getUserIdByName(new_user_name);
        if (uid != UINT64_MAX) {
            _connections[uid] = hdl;
            _storage->registerUser(uid);
            logger::logger << logger::debug << "ChatClientDatabase::regiserClinetSession with id = " << uid << "." << logger::endl;
            return uid;
        }

        std::optional<uint64_t> userId = _storage->getUserIdByName(new_user_name);
        if (!userId) {
            userId = _storage->addUser(new_user_name);
        }
        _connections[*userId] = hdl;
        _storage->registerUser(*userId);
        logger::logger << logger::debug << "ChatClientDatabase::regiserClinetSession with id = " << *userId << "." << logger::endl;
        return *userId;
    }

    bool ChatClientDatabase::unregiserClinet(uint64_t user_id) {
        return _storage->unregister(user_id);
    }

    websocketpp::connection_hdl ChatClientDatabase::connection(uint64_t user_id) {
        auto it = _connections.find(user_id);
        if (it != _connections.end()) {
            return it->second;
        }
        return {};
    }

    uint64_t ChatClientDatabase::getUserIdByName(const std::string& uname) {
        auto userId = _storage->getUserIdByName(uname);
        if (!userId) {
            return UINT64_MAX;
        }
        return *userId;
    }

    bool ChatClientDatabase::hasMsg(uint64_t userId) {
        return _storage->hasMsg(userId);
    }

    bool ChatClientDatabase::isRegistered(uint64_t userId) {
        return _storage->isUserRegistered(userId);
    }

    bool ChatClientDatabase::saveMsg(uint64_t to, uint64_t from, const std::string& message, bool wasSend) {
        return _storage->saveMsg(to, from, message, wasSend);
    }

    bool ChatClientDatabase::createConnection(uint64_t from, uint64_t to) {
        return _storage->createConnection(from, to);
    }

    WChat::ChatServer::core::storage::MsgHolder ChatClientDatabase::popMsg(uint64_t user_id) {
        return _storage->popMsg(user_id);
    }

    WChat::ChatServer::core::storage::Contacts ChatClientDatabase::getContacts(uint64_t userId) {
        return _storage->getContacts(userId);
    }

    uint64_t ChatClientDatabase::regiserClinetSession(uint64_t user_id) {
        _storage->registerUser(user_id);
        logger::logger << logger::debug << "ChatClientDatabase::regiserClinetSession with id = " << user_id << "." << logger::endl;
        return user_id;
    }

};  // namespace WChat::ChatServer::client
