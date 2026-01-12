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

#include "Message.h"

#include <memory>
#include <string>

#include "logger.h"
#include "server/client/ChatClientDatabase.h"
#include "server/errors/ErrorHandlers.h"
namespace WChat::ChatServer::messages::handlers {
    Message::Message() {
    }

    Message::~Message() {
    }

    void Message::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        if (!msg.has_textmessage()) {
            throw WChat::ChatServer::errors::ProtoculError("Msg don't contain TxtMsg");
        }
        uint64_t from       = msg.textmessage().from_user_id();
        uint64_t to         = msg.textmessage().to_user_id();
        std::string message = msg.textmessage().message();
        send_msg_to_user(s, WChat::ChatServer::client::ChatClientDatabase::getInstance().connection(from), from, to, message);

        logger::logger << logger::debug << "MessageHandler_Message::handle: from=`" << from << "`; to=`" << to << "`; msg=`" << message << "`." << logger::endl;

        if (WChat::ChatServer::client::ChatClientDatabase::getInstance().isRegistered(to)) {
            send_msg_to_user(s, WChat::ChatServer::client::ChatClientDatabase::getInstance().connection(to), from, to, message);
            WChat::ChatServer::client::ChatClientDatabase::getInstance().saveMsg(to, from, message, true);
        } else {
            WChat::ChatServer::client::ChatClientDatabase::getInstance().saveMsg(to, from, message, false);
        }
    }
}  // namespace WChat::ChatServer::messages::handlers
