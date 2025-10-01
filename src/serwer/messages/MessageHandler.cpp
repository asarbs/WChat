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

#include "MessageHandler.h"

#include <string>

#include "logger.h"

MessageHandler::MessageHandler() {
}

MessageHandler::~MessageHandler() {
}

void send_msg(server* s, websocketpp::connection_hdl hdl, const std::string& msg) {
    logger::logger << "send_msg: msg:" << logger::debug << msg << logger::endl;
    s->send(hdl, msg, websocketpp::frame::opcode::text);
}

void send_ack_nack(server* s, websocketpp::connection_hdl hdl, bool status) {
    nlohmann::json j;
    j["msg_type_id"]       = 0;
    j["payload"]["status"] = status ? "ok" : "not";

    std::string msg_str = j.dump();
    send_msg(s, hdl, msg_str);
}
void send_ack(server* s, websocketpp::connection_hdl hdl) {
    send_ack_nack(s, hdl, true);
}
void send_nack(server* s, websocketpp::connection_hdl hdl) {
    send_ack_nack(s, hdl, false);
}
