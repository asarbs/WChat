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

#include "SendMsg.h"

#include "logger.h"

namespace WChat::internal::cui::excutor {

    SendMsg::SendMsg(std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toServerQueue, std::shared_ptr<WChat::ChatClient::ChatClient> client)
        : _toServerQueue(toServerQueue), _client(client) {
    }

    SendMsg::~SendMsg() {
    }

    SendMsg::SendMsg(const SendMsg& rhs) : _toServerQueue(rhs._toServerQueue), _client(rhs._client) {
    }

    SendMsg::SendMsg(const SendMsg&& rhs) : _toServerQueue(std::move(rhs._toServerQueue)), _client(std::move(rhs._client)) {
    }

    SendMsg& SendMsg::operator=(const SendMsg& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = rhs._toServerQueue;
            this->_client        = rhs._client;
        }
        return *this;
    }

    SendMsg& SendMsg::operator=(const SendMsg&& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = std::move(rhs._toServerQueue);
            this->_client        = std::move(rhs._client);
        }
        return *this;
    }

    bool SendMsg::operator()(const std::string& args) {
        logger::logger << logger::info << args << logger::endl;
        std::string::size_type sz = args.find_first_of(" ");
        std::string addressee     = args.substr(0, sz);
        std::string message       = args.substr(sz);

        logger::logger << logger::info << addressee << "->" << message << logger::endl;

        WChat::ChatClient::server::api::ProtoBuffer buff = WChat::ChatClient::server::api::buildTestMsg(_client->getUserId(), 1, message);
        _toServerQueue->push(buff);

        return true;
    }
};  // namespace WChat::internal::cui::excutor
