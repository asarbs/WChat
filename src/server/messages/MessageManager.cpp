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

#include "MessageManager.h"

#include "logger.h"

namespace WChat::ChatServer::messages {
    MessageManager::MessageManager() {
    }

    MessageManager::~MessageManager() {
    }

    void MessageManager::handle(websocket_server* s, const websocketpp::connection_hdl& hdl, WChat::Msg msg) {
        WChat::MessageType message_type_id = msg.type();
        logger::logger << logger::debug << "Looking for message_type_id = `" << message_type_id << "` handler." << logger::endl;
        auto f_handler = _handlers.find(message_type_id);
        if (f_handler == _handlers.end()) {
            logger::logger << logger::debug << "Can't find handler for message_type_id = `" << message_type_id << "`." << logger::endl;
            handlers::send_nack(s, hdl);
            return;
        }
        f_handler->second->handle(s, hdl, msg);
    }

    void MessageManager::register_handler(WChat::MessageType message_type_id, std::shared_ptr<handlers::Handler> handler) {
        auto f_handler = _handlers.find(message_type_id);
        if (f_handler == _handlers.end()) {
            logger::logger << logger::debug << "Register new handler for message_type_id " << message_type_id << "." << logger::endl;
            _handlers[message_type_id] = handler;
        } else {
            logger::logger << logger::warning << "message_type_id = `" << message_type_id << "` occupied." << logger::endl;
        }
    }
};  // namespace WChat::ChatServer::messages
