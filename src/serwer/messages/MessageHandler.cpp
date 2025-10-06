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
    logger::logger << logger::debug << "send_msg: msg:" << logger::debug << msg << logger::endl;
    s->send(hdl, msg, websocketpp::frame::opcode::text);
}

void send_ack_nack(server* s, websocketpp::connection_hdl hdl, bool status) {
    nlohmann::json j;
    j["msg_type_id"]       = 0;
    j["payload"]["status"] = status ? "ok" : "not";

    std::string msg_str = j.dump();
    send_msg(s, hdl, msg_str);
}
void send_ack(server* s, websocketpp::connection_hdl hdl) {  // cppcheck-suppress unusedFunction
    send_ack_nack(s, hdl, true);
}
void send_nack(server* s, websocketpp::connection_hdl hdl) {
    send_ack_nack(s, hdl, false);
}

void send_user_registration(server* s, websocketpp::connection_hdl hdl, const std::string& user_name, uint64_t user_db_id) {
    nlohmann::json j;
    j["msg_type_id"]          = 0;
    j["payload"]["status"]    = "ok";
    j["payload"]["user_name"] = user_name;
    j["payload"]["user_id"]   = user_db_id;

    std::string msg_str = j.dump();
    send_msg(s, hdl, msg_str);
}

void send_msg_to_user(server* s, websocketpp::connection_hdl hdl, uint64_t user_id_from, uint64_t user_id_to, const std::string& msg) {
    nlohmann::json j;
    j["msg_type_id"]        = 3;
    j["payload"]["from"]    = user_id_from;
    j["payload"]["to"]      = user_id_to;
    j["payload"]["message"] = msg;

    std::string msg_str = j.dump();
    send_msg(s, hdl, msg_str);
}
