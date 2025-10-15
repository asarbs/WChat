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

#include <memory>
#include <string>

#include "logger.h"
#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/errors/ErrorHandlers.h"
namespace WChat::ChatServer::messages::handlers {
    MessageHandler_Message::MessageHandler_Message() {
    }

    MessageHandler_Message::~MessageHandler_Message() {
    }

    void MessageHandler_Message::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        if (!msg.has_textmessage()) {
            throw WChat::ChatServer::errors::ProtoculError("Msg don't contain TxtMsg");
        }
        uint64_t from       = msg.textmessage().from_user_id();
        uint64_t to         = msg.textmessage().to_user_id();
        std::string message = msg.textmessage().message();

        std::shared_ptr<WChat::ChatServer::client::ChatClient> from_user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(from);
        if (from_user == nullptr) {
            logger::logger << logger::warning << "Can't find addressee user with id `" << to << "`" << logger::endl;
            send_nack(s, hdl);
            return;
        }
        send_msg_to_user(s, hdl, from, to, message);

        logger::logger << logger::debug << "MessageHandler_Message::handle: from=`" << from << "`; to=`" << to << "`; msg=`" << message << "`." << logger::endl;
        std::shared_ptr<WChat::ChatServer::client::ChatClient> to_user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(to);
        if (to_user == nullptr) {
            logger::logger << logger::warning << "Can't find addressee user with id `" << to << "`" << logger::endl;
            return;
        }

        if (to_user->isRegistered()) {
            send_msg_to_user(s, to_user->connection, from, to, message);
        } else {
            to_user->saveMsg(from, message);
        }
    }
}  // namespace WChat::ChatServer::messages::handlers
