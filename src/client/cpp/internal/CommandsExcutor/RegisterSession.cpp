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

#include "RegisterSession.h"

#include "logger.h"
#include "server/api/ProtoWrapper.h"
#include "server/core/Config.h"
#include "server/core/ConfigChoice.h"
#include "server/core/ConfigParameter.h"

namespace WChat::internal::cui::excutor {
    RegisterSession::RegisterSession(std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toServerQueue, std::shared_ptr<WChat::ChatClient::ChatClient> client)
        : _toServerQueue(toServerQueue), _client(client) {
    }

    RegisterSession::~RegisterSession() {
    }

    RegisterSession::RegisterSession(const RegisterSession& rhs) : _toServerQueue(rhs._toServerQueue), _client(rhs._client) {
    }

    RegisterSession::RegisterSession(const RegisterSession&& rhs) : _toServerQueue(std::move(rhs._toServerQueue)), _client(std::move(rhs._client)) {
    }

    RegisterSession& RegisterSession::operator=(const RegisterSession& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = rhs._toServerQueue;
            this->_client        = rhs._client;
        }
        return *this;
    }

    RegisterSession& RegisterSession::operator=(const RegisterSession&& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = std::move(rhs._toServerQueue);
            this->_client        = std::move(rhs._client);
        }
        return *this;
    }

    bool RegisterSession::operator()(const std::string& args) {
        _client->setName(args);
        std::shared_ptr<WChat::ChatServer::core::ConfigParameter> userNameParam =
            WChat::ChatServer::core::ServerConfig::instance().get(WChat::ChatServer::core::ParamKey::UserName);
        userNameParam->set(_client->getName());
        WChat::ChatServer::core::ServerConfig::instance().saveToFile();

        WChat::ChatClient::server::api::ProtoBuffer buff = WChat::ChatClient::server::api::buildRegisterSessionReq(args);

        _toServerQueue->push(buff);
        return true;
    }
};  // namespace WChat::internal::cui::excutor
