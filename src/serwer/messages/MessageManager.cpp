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

void MessageManager::handle(uint32_t message_id, nlohmann::json::reference payload) {
    logger::logger << logger::error << "Looking for message_id = `" << message_id << "` handler." << logger::endl;
    auto f_handler = _handlers.find(message_id);
    if (f_handler == _handlers.end()) {
        logger::logger << logger::error << "Can't find handler for message_id = `" << message_id << "`." << logger::endl;
        return;
    }
    // f_handler->second->handle(payload);
}

void MessageManager::register_handler(uint32_t message_id, std::shared_ptr<MessageHandler> handler) {
    auto f_handler = _handlers.find(message_id);
    if (f_handler == _handlers.end()) {
        _handlers[message_id] = handler;
    } else {
        logger::logger << logger::error << "Message_id = `" << message_id << "` occupied." << logger::endl;
    }
}
