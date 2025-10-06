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

#include "MessageHandler_Message.h"

#include <optional>
#include <string>

#include "logger.h"
#include "serwer/ChatClient.h"
#include "serwer/ChatClientDatabase.h"

MessageHandler_Message::MessageHandler_Message() {
}

MessageHandler_Message::~MessageHandler_Message() {
}

void MessageHandler_Message::handle(server* s, const websocketpp::connection_hdl& hdl, nlohmann::json::reference payload) {
    uint64_t from       = payload.at("from").get<uint64_t>();
    uint64_t to         = payload.at("to").get<uint64_t>();
    std::string message = payload.at("message").get<std::string>();

    logger::logger << logger::debug << "MessageHandler_Message::handle: from=`" << from << "`; to=`" << to << "`; msg=`" << message << "`." << logger::endl;
    // std::optional<std::reference_wrapper<ChatClient>> to_user = ChatClientDatabase::getInstance().get(to);

    send_msg_to_user(s, hdl, from, to, message);
}
