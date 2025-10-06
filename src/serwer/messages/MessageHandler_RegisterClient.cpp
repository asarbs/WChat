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

#include "MessageHandler_RegisterClient.h"

#include <optional>
#include <string>

#include "logger.h"
#include "serwer/ChatClient.h"
#include "serwer/ChatClientDatabase.h"

MessageHandler_RegisterClient::MessageHandler_RegisterClient() {
}

MessageHandler_RegisterClient::~MessageHandler_RegisterClient() {
}

void MessageHandler_RegisterClient::handle(server* s, const websocketpp::connection_hdl& hdl, nlohmann::json::reference payload) {
    std::string user_name = payload["user_name"];
    uint64_t new_user_id  = ChatClientDatabase::getInstance().regiserClinet(hdl, user_name);
    logger::logger << logger::debug << "Register new Client: user_name=" << user_name << "; DB ID=" << new_user_id << logger::endl;
    send_user_registration(s, hdl, user_name, new_user_id);
}
