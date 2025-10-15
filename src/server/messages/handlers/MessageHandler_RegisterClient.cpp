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

#include <memory>
#include <optional>
#include <string>

#include "logger.h"
#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/errors/ErrorHandlers.h"
#include "server/messages/handlers/MessageHandler.h"
namespace WChat::ChatServer::messages::handlers {
    MessageHandler_RegisterClient::MessageHandler_RegisterClient() {
    }

    MessageHandler_RegisterClient::~MessageHandler_RegisterClient() {
    }

    void MessageHandler_RegisterClient::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        if (!msg.has_registersessionreq()) {
            throw WChat::ChatServer::errors::ProtoculError("Msg don't contain RegisterSessionReq");
        }
        std::string user_name = msg.registersessionreq().user_name();
        uint64_t new_user_id  = WChat::ChatServer::client::ChatClientDatabase::getInstance().regiserClinet(hdl, user_name);
        logger::logger << logger::debug << "Register new Client: user_name=" << user_name << "; DB ID=" << new_user_id << logger::endl;
        send_user_registration(s, hdl, user_name, new_user_id);

        std::shared_ptr<WChat::ChatServer::client::ChatClient> from_user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(new_user_id);
        if (from_user != nullptr) {
            while (from_user->hasMsg()) {
                WChat::ChatServer::client::ChatClient::MsgHolder tmpMsg = from_user->popMsg();
                logger::logger << logger::debug << "Send waiting msg from " << tmpMsg.from << ":" << tmpMsg.message << logger::endl;
                send_msg_to_user(s, hdl, tmpMsg.from, new_user_id, tmpMsg.message);
            }
        } else {
            logger::logger << logger::warning << "User object for id = " << new_user_id << " is NULL" << logger::endl;
        }
    }
};  // namespace WChat::ChatServer::messages::handlers
