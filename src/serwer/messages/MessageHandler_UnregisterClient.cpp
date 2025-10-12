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
#include "serwer/ChatClient.h"
#include "serwer/ChatClientDatabase.h"
#include "serwer/ErrorHandlers.h"
#include "serwer/messages/MessageHandler.h"

MessageHandler_UnregisterClient::MessageHandler_UnregisterClient() {
}

MessageHandler_UnregisterClient::~MessageHandler_UnregisterClient() {
}

void MessageHandler_UnregisterClient::handle(server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
    if (!msg.has_unregistersessionreq()) {
        throw wchat::protocul::ProtoculError("Msg don't contain UnregisterSessionReq");
    }
    uint64_t user_id = msg.unregistersessionreq().user_id();
    if (ChatClientDatabase::getInstance().unregiserClinet(user_id)) {
        send_ack(s, hdl);
    } else {
        send_nack(s, hdl);
    }
}
