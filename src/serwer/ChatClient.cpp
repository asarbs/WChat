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

ChatClient::ChatClient() : __user_id(UINT64_MAX) {
}
ChatClient::ChatClient(uint64_t user_id) : __user_id(user_id) {
}
ChatClient::ChatClient(const ChatClient& other) {
    this->__user_id  = other.__user_id;
    this->connection = other.connection;
}
ChatClient& ChatClient::operator=(const ChatClient& other) {
    this->__user_id  = other.__user_id;
    this->connection = other.connection;
    return *this;
}
ChatClient::~ChatClient() {
}
