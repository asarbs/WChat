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
#include "serwer/ErrorHandlers.h"

MessageHandler_Message::MessageHandler_Message() {
}

MessageHandler_Message::~MessageHandler_Message() {
}

void MessageHandler_Message::handle(server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
    if (!msg.has_textmessage()) {
        throw wchat::protocul::ProtoculError("Msg don't contain TxtMsg");
    }
    uint64_t from       = msg.textmessage().from_user_id();
    uint64_t to         = msg.textmessage().to_user_id();
    std::string message = msg.textmessage().message();

    logger::logger << logger::debug << "MessageHandler_Message::handle: from=`" << from << "`; to=`" << to << "`; msg=`" << message << "`." << logger::endl;
    const ChatClient& to_user = ChatClientDatabase::getInstance().get(to).value().get();

    send_msg_to_user(s, hdl, from, to, message);
    send_msg_to_user(s, to_user.connection, from, to, message);
}
