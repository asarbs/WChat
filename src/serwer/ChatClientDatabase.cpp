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

#include "ChatClientDatabase.h"

#include "logger.h"

ChatClientDatabase& ChatClientDatabase::getInstance() {
    static ChatClientDatabase instance;
    return instance;
}

ChatClientDatabase::ChatClientDatabase() : __next_free_user_id(0) {
}

// ChatClientDatabase::~ChatClientDatabase() {
// }

uint64_t ChatClientDatabase::regiserClinet(websocketpp::connection_hdl hdl, const std::string& new_user_name) {
    ChatClient cc(__next_free_user_id, new_user_name);
    cc.connection                       = hdl;
    __chat_clients[__next_free_user_id] = cc;
    uint64_t current_client_id          = __next_free_user_id;
    logger::logger << logger::debug << "ChatClientDatabase::regiserClinet with id = " << __next_free_user_id << "." << logger::endl;
    __next_free_user_id++;
    return current_client_id;
}

void ChatClientDatabase::unregiserClinet(uint64_t user_id) {  // cppcheck-suppress unusedFunction
    __chat_clients.extract(user_id);
}

std::optional<std::reference_wrapper<ChatClient>> ChatClientDatabase::get(uint64_t user_id) {  // cppcheck-suppress unusedFunction
    std::map<uint64_t, ChatClient>::iterator it = __chat_clients.find(user_id);
    if (it != __chat_clients.end()) {
        return it->second;
    }
    return std::nullopt;
}

void ChatClientDatabase::clean() {  // cppcheck-suppress unusedFunction
    __chat_clients.clear();
    __next_free_user_id = 0;
}
