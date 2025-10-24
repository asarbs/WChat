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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "server/client/ChatClient.h"
#include "server/proto/messeges.pb.h"

typedef websocketpp::server<websocketpp::config::asio> websocket_server;

namespace WChat::ChatServer::messages::handlers {
    class Handler {
        public:
            Handler();
            virtual ~Handler();
            virtual void handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) = 0;

        protected:
        private:
    };

    void send_msg(websocket_server* s, websocketpp::connection_hdl hdl, const WChat::Msg& msg);
    void send_msg(websocket_server* s, websocketpp::connection_hdl hdl, const std::string& msg);
    void send_ack(websocket_server* s, websocketpp::connection_hdl hdl);
    void send_nack(websocket_server* s, websocketpp::connection_hdl hdl);
    void send_user_registration(websocket_server* s, websocketpp::connection_hdl hdl, const std::string& user_name, uint64_t user_db_id);
    void send_msg_to_user(websocket_server* s, websocketpp::connection_hdl hdl, uint64_t user_id_from, uint64_t user_id_to, const std::string& msg);
    void send_user_contacts(websocket_server* s,                                                                         //
                            websocketpp::connection_hdl hdl,                                                             //
                            std::map<uint64_t, std::shared_ptr<WChat::ChatServer::client::ChatClient>>::iterator begin,  //
                            std::map<uint64_t, std::shared_ptr<WChat::ChatServer::client::ChatClient>>::iterator end);
};  // namespace WChat::ChatServer::messages::handlers
#endif
