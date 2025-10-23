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

#include "ConnectionRes.h"

#include <memory>
#include <utility>

#include "logger.h"
#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/errors/ErrorHandlers.h"

namespace WChat::ChatServer::messages::handlers {

    ConnectionRes::ConnectionRes() {
    }

    ConnectionRes::~ConnectionRes() {
    }

    void ConnectionRes::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        if (!msg.has_contactconnectionres()) {
            throw WChat::ChatServer::errors::ProtoculError("Msg don't contain ContactConnectionRes");
        }
        WChat::Response status = msg.contactconnectionres().status();
        uint64_t to_user_id    = std::move(msg.contactconnectionres().to_user_id());
        uint64_t from_user_id  = std::move(msg.contactconnectionres().from_user_id());
        logger::logger << logger::info << "status=" << status << "; to_user_id=" << to_user_id << "; from_user_id=" << from_user_id << logger::endl;
        if (status == WChat::Response::ACK) {
            send_ack(s, hdl);
            std::shared_ptr<WChat::ChatServer::client::ChatClient> from_user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(from_user_id);
            send_msg(s, from_user->connection, msg);
            std::shared_ptr<WChat::ChatServer::client::ChatClient> to_user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(to_user_id);
            from_user->addContect(to_user);
            to_user->addContect(from_user);
        } else {
            send_nack(s, hdl);
            std::shared_ptr<WChat::ChatServer::client::ChatClient> from_user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(from_user_id);
            send_msg(s, from_user->connection, msg);
        }
    }
};  // namespace WChat::ChatServer::messages::handlers
