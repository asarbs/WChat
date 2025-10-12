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

#include "ChatClient.h"

#include "logger.h"

ChatClient::ChatClient() : __user_id(UINT64_MAX), __is_registered(false) {
}
ChatClient::ChatClient(uint64_t user_id, const std::string& name) : __user_id(user_id), __name(name), __is_registered(false) {
}
ChatClient::ChatClient(const ChatClient& other) {
    this->__user_id       = other.__user_id;
    this->connection      = other.connection;
    this->__is_registered = other.__is_registered;
}
ChatClient& ChatClient::operator=(const ChatClient& other) {
    this->__user_id       = other.__user_id;
    this->connection      = other.connection;
    this->__is_registered = other.__is_registered;
    return *this;
}
ChatClient::~ChatClient() {
}
void ChatClient::registerClient() {
    __is_registered = true;
}
void ChatClient::unregister() {
    __is_registered = false;
}
