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

#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <cstdint>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

#include "server/messages/handlers/Handler.h"
#include "server/proto/messeges.pb.h"

#pragma once
namespace WChat::ChatServer::messages {
    class Manager {
        public:
            Manager();
            ~Manager();
            void register_handler(WChat::MessageType message_id, std::shared_ptr<handlers::Handler> handler);
            void handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg payload);

        protected:
        private:
            std::map<WChat::MessageType, std::shared_ptr<handlers::Handler> > _handlers;
    };
};  // namespace WChat::ChatServer::messages
#endif
