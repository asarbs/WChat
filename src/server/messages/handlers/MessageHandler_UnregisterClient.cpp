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

#include "MessageHandler_UnregisterClient.h"

#include "logger.h"
#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/errors/ErrorHandlers.h"
#include "server/messages/handlers/MessageHandler.h"
namespace WChat::ChatServer::messages::handlers {
    UnregisterClient::UnregisterClient() {
    }

    UnregisterClient::~UnregisterClient() {
    }

    void UnregisterClient::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        if (!msg.has_unregistersessionreq()) {
            throw WChat::ChatServer::errors::ProtoculError("Msg don't contain UnregisterSessionReq");
        }
        uint64_t user_id = msg.unregistersessionreq().user_id();
        if (WChat::ChatServer::client::ChatClientDatabase::getInstance().unregiserClinet(user_id)) {
            send_ack(s, hdl);
        } else {
            send_nack(s, hdl);
        }
    }
};  // namespace WChat::ChatServer::messages::handlers
