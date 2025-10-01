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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#pragma once

#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

class MessageHandler {
    public:
        MessageHandler();
        virtual ~MessageHandler();
        virtual void handle(server* s, websocketpp::connection_hdl& hdl, nlohmann::json::reference payload) = 0;

    protected:
    private:
};

void send_msg(server* s, websocketpp::connection_hdl hdl, const std::string& msg);
void send_ack(server* s, websocketpp::connection_hdl hdl);
void send_nack(server* s, websocketpp::connection_hdl hdl);

#endif
