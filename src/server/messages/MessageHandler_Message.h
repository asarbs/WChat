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

#ifndef MESSAGEHANDLER_MESSAGE_H
#define MESSAGEHANDLER_MESSAGE_H

#pragma once

#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/messages/MessageHandler.h"
namespace WChat::ChatServer::messages {
    class MessageHandler_Message : public MessageHandler {
        public:
            MessageHandler_Message();
            ~MessageHandler_Message() override;
            void handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg payload) override;

        protected:
        private:
    };
};  // namespace WChat::ChatServer::messages
#endif
