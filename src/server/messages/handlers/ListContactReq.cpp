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

#include "ListContactReq.h"

#include <memory>
#include <utility>

#include "logger.h"
#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/errors/ErrorHandlers.h"

namespace WChat::ChatServer::messages::handlers {
    ListContactReq::ListContactReq() {
    }

    ListContactReq::~ListContactReq() {
    }

    void ListContactReq::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        if (!msg.has_listcontactreq()) {
            throw WChat::ChatServer::errors::ProtoculError("Msg don't contain ContactConnectionReq");
        }

        uint64_t userId                                             = std::move(msg.listcontactreq().user_id());
        std::shared_ptr<WChat::ChatServer::client::ChatClient> user = WChat::ChatServer::client::ChatClientDatabase::getInstance().get(userId);
        send_user_contacts(s, hdl, user->contactsBegin(), user->contactsEnd());
    }
};  // namespace WChat::ChatServer::messages::handlers
