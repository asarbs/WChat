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

MessageManager::MessageManager() {
}

MessageManager::~MessageManager() {
}

void MessageManager::handle(server* s, websocketpp::connection_hdl& hdl, uint32_t message_type_id, nlohmann::json::reference payload) {
    logger::logger << logger::debug << "Looking for message_type_id = `" << message_type_id << "` handler." << logger::endl;
    auto f_handler = _handlers.find(message_type_id);
    if (f_handler == _handlers.end()) {
        logger::logger << logger::debug << "Can't find handler for message_type_id = `" << message_type_id << "`." << logger::endl;
        send_nack(s, hdl);
        return;
    }
    f_handler->second->handle(s, hdl, payload);
}

void MessageManager::register_handler(uint32_t message_type_id, std::shared_ptr<MessageHandler> handler) {
    auto f_handler = _handlers.find(message_type_id);
    if (f_handler == _handlers.end()) {
        logger::logger << logger::debug << "Register new handler for message_type_id " << message_type_id << "." << logger::endl;
        _handlers[message_type_id] = handler;
    } else {
        logger::logger << logger::warning << "message_type_id = `" << message_type_id << "` occupied." << logger::endl;
    }
}
