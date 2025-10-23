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

#include "ChatClient.h"

#include <utility>

#include "logger.h"

namespace WChat::ChatServer::client {

    ChatClient::ChatClient() : _user_id(UINT64_MAX), _is_registered(false) {
    }
    ChatClient::ChatClient(uint64_t user_id, const std::string& name) : _user_id(user_id), _name(name), _is_registered(false) {
    }
    ChatClient::ChatClient(const ChatClient& other) {
        this->_user_id       = other._user_id;
        this->connection     = other.connection;
        this->_is_registered = other._is_registered;
    }
    ChatClient& ChatClient::operator=(const ChatClient& other) {
        this->_user_id       = other._user_id;
        this->connection     = other.connection;
        this->_is_registered = other._is_registered;
        return *this;
    }
    ChatClient::~ChatClient() {
    }

    bool ChatClient::isRegistered() const {
        return _is_registered;
    }
    uint64_t ChatClient::getUserId() const {  // cppcheck-suppress unusedFunction
        return _user_id;
    }
    const std::string& ChatClient::getName() const {
        return _name;
    }

    void ChatClient::registerClient() {
        _is_registered = true;
    }
    void ChatClient::unregister() {
        _is_registered = false;
    }
    void ChatClient::saveMsg(uint64_t from, std::string message) {
        _savedMsg.push_back({from, message});
    }
    bool ChatClient::hasMsg() const {
        return !_savedMsg.empty();
    }
    ChatClient::MsgHolder ChatClient::popMsg() {
        MsgHolder tmp = std::move(_savedMsg.front());
        _savedMsg.pop_front();
        return tmp;
    }
    void ChatClient::addContect(std::shared_ptr<ChatClient> contact) {
        logger::logger << logger::info << getName() << " has new contact " << contact->getName() << logger::endl;
        _contacts[contact->_user_id] = contact;
    }

    std::map<uint64_t, std::shared_ptr<ChatClient>>::iterator ChatClient::contactsBegin() {  // cppcheck-suppress unusedFunction
        return _contacts.begin();
    }
    std::map<uint64_t, std::shared_ptr<ChatClient>>::iterator ChatClient::contactsEnd() {  // cppcheck-suppress unusedFunction
        return _contacts.end();
    }

};  // namespace WChat::ChatServer::client
