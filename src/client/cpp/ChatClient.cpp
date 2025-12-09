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

#include <utility>

#include "Config.h"
#include "ConfigChoice.h"
#include "ConfigParameter.h"
#include "internal/ClientConfig.h"
#include "logger.h"

namespace WChat::ChatClient {
    ChatClient::ChatClient() : _userId(UINT64_MAX) {
        std::string confFileName = WChat::internal::Config::ClientConfig::instance().value<std::string>(WChat::internal::Config::ParamKey::UserName);
        if (confFileName == "username") {
            logger::logger << logger::info << "Please regiser in server with 'reg' command" << logger::endl;
        } else {
            _name = confFileName;
        }
    }

    ChatClient::~ChatClient() {
    }

    ChatClient::ChatClient(const ChatClient& rhs) : _userId(rhs._userId) {
    }

    ChatClient::ChatClient(const ChatClient&& rhs) : _userId(std::move(rhs._userId)) {
    }

    ChatClient& ChatClient::operator=(const ChatClient& rhs) {
        if (this != &rhs) {
            _userId = rhs._userId;
        }
        return *this;
    }

    ChatClient& ChatClient::operator=(const ChatClient&& rhs) {
        if (this != &rhs) {
            _userId = std::move(rhs._userId);
        }
        return *this;
    }

    void ChatClient::setUserId(uint64_t userId) {
        logger::logger << logger::debug << "Set User " << _userId << "->" << userId << "." << logger::endl;
        _userId = userId;
    }
    uint64_t ChatClient::getUserId() const {
        return _userId;
    }
    bool ChatClient::isRegistered() const {
        return _userId != UINT64_MAX;
    }
    void ChatClient::setName(const std::string& name) {
        _name = name;
    }
    const std::string& ChatClient::getName() const {
        return _name;
    }

    bool ChatClient::hasName() const {
        return _name == "username";
    }
};  // namespace WChat::ChatClient
