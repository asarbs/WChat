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

#include <google/protobuf/util/json_util.h>

#include <string>

#include "logger.h"
#include "serwer/proto/messeges.pb.h"

MessageHandler::MessageHandler() {
}

MessageHandler::~MessageHandler() {
}

void send_msg(server* s, websocketpp::connection_hdl hdl, const std::string& msg) {  // cppcheck-suppress unusedFunction
    logger::logger << logger::debug << "send_msg: msg:" << logger::debug << msg << logger::endl;
    s->send(hdl, msg, websocketpp::frame::opcode::text);
}

void send_ack_nack(server* s, websocketpp::connection_hdl hdl, bool status) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::RESPONSE);
    msg.set_response(status ? WChat::Response::ACK : WChat::Response::NACK);
    std::string serialized;
    msg.SerializeToString(&serialized);
    s->send(hdl, serialized, websocketpp::frame::opcode::binary);
}

void send_ack(server* s, websocketpp::connection_hdl hdl) {  // cppcheck-suppress unusedFunction
    send_ack_nack(s, hdl, true);
}
void send_nack(server* s, websocketpp::connection_hdl hdl) {
    send_ack_nack(s, hdl, false);
}

void send_user_registration(server* s, websocketpp::connection_hdl hdl, const std::string& user_name, uint64_t user_db_id) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_RES);
    WChat::RegisterSessionRes* rsr = msg.mutable_registersessionres();
    rsr->set_user_id(user_db_id);
    rsr->set_status(WChat::Response::ACK);
    std::string serialized;
    msg.SerializeToString(&serialized);
    s->send(hdl, serialized, websocketpp::frame::opcode::binary);
}

void send_msg_to_user(server* s, websocketpp::connection_hdl hdl, uint64_t user_id_from, uint64_t user_id_to, const std::string& msg_text) {
    WChat::Msg msg_resp;
    msg_resp.set_version(1);
    msg_resp.set_type(WChat::MessageType::SEND_TEXT_MSG);
    WChat::TextMessage* txtMsg = msg_resp.mutable_textmessage();
    txtMsg->set_to_user_id(user_id_to);
    txtMsg->set_from_user_id(user_id_from);
    txtMsg->set_message(msg_text);

    std::string serialized;
    msg_resp.SerializeToString(&serialized);
    s->send(hdl, serialized, websocketpp::frame::opcode::binary);
}
