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

#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <cstdint>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

#include "serwer/messages/MessageHandler.h"

#pragma once

class MessageManager {
    public:
        MessageManager();
        ~MessageManager();
        void register_handler(uint32_t message_id, std::shared_ptr<MessageHandler> handler);
        void handle(uint32_t message_id, nlohmann::json::reference payload);

    protected:
    private:
        std::map<uint32_t, std::shared_ptr<MessageHandler> > _handlers;
};

#endif
