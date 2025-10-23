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

#include "ConnectionReq.h"

#include <memory>
#include <string>
#include <utility>

#include "logger.h"
#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/errors/ErrorHandlers.h"

namespace WChat::ChatServer::messages::handlers {

    ConnectionReq::ConnectionReq() {
    }

    ConnectionReq::~ConnectionReq() {
    }

    void ConnectionReq::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        if (!msg.has_contactconnectionreq()) {
            throw WChat::ChatServer::errors::ProtoculError("Msg don't contain ContactConnectionReq");
        }

        std::string name      = std::move(msg.contactconnectionreq().from_user_name());
        uint64_t to_user_id   = std::move(msg.contactconnectionreq().to_user_id());
        uint64_t from_user_id = std::move(msg.contactconnectionreq().from_user_id());
        logger::logger << logger::info << "ConnectionReq: name=" << name << "; to_user_id=" << to_user_id << "; from_user_id=" << from_user_id << logger::endl;
        send_ack(s, hdl);

        std::shared_ptr<WChat::ChatServer::client::ChatClient> to_user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(to_user_id);
        send_msg(s, to_user->connection, msg);
    }
};  // namespace WChat::ChatServer::messages::handlers
