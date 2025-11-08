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

#include "ChatClient.h"

#include "logger.h"

namespace WChat::ChatClient {
    ChatClient::ChatClient() {
    }

    ChatClient::~ChatClient() {
    }

    ChatClient::ChatClient(const ChatClient& rhs) {
    }

    ChatClient::ChatClient(const ChatClient&& rhs) {
    }

    ChatClient& ChatClient::operator=(const ChatClient& rhs) {
        if (this != &rhs) {
            //
        }
        return *this;
    }

    ChatClient& ChatClient::operator=(const ChatClient&& rhs) {
        if (this != &rhs) {
            //
        }
        return *this;
    }
};  // namespace WChat::ChatClient
