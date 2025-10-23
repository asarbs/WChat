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

#ifndef MESSAGEHANDLER_UNREGISTERCLIENT_H
#define MESSAGEHANDLER_UNREGISTERCLIENT_H

#pragma once

#include "server/messages/handlers/Handler.h"

namespace WChat::ChatServer::messages::handlers {

    class UnregisterClient : public Handler {
        public:
            UnregisterClient();
            ~UnregisterClient() override;
            void handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) override;

        protected:
        private:
    };
};  // namespace WChat::ChatServer::messages::handlers
#endif
