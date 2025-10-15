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

#ifndef MESSAGEHANDLER_REGISTERCLIENT_H
#define MESSAGEHANDLER_REGISTERCLIENT_H

#pragma once

#include "server/messages/handlers/MessageHandler.h"
namespace WChat::ChatServer::messages::handlers {
    class MessageHandler_RegisterClient : public MessageHandler {
        public:
            MessageHandler_RegisterClient();
            ~MessageHandler_RegisterClient() override;
            void handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) override;

        protected:
        private:
    };
};  // namespace WChat::ChatServer::messages::handlers
#endif
