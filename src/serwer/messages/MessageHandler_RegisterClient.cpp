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

#include "logger.h"

MessageHandler_RegisterClient::MessageHandler_RegisterClient() {
}

MessageHandler_RegisterClient::~MessageHandler_RegisterClient() {
}

void MessageHandler_RegisterClient::handle(server* s, websocketpp::connection_hdl& hdl, nlohmann::json::reference payload) {
    logger::logger << logger::debug << "MessageHandler_RegisterClient::handle" << logger::endl;
    send_ack(s, hdl);
}
