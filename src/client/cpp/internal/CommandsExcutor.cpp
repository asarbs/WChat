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

#include "CommandsExcutor.h"

#include "logger.h"
#include "server/api/ProtoWrapper.h"

namespace WChat::internal::cui {
    CommandsExcutor::CommandsExcutor(std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toServerQueue) : _toServerQueue(toServerQueue) {
    }

    CommandsExcutor::~CommandsExcutor() {
    }

    CommandsExcutor::CommandsExcutor(const CommandsExcutor& rhs) : _toServerQueue(rhs._toServerQueue) {
    }

    CommandsExcutor::CommandsExcutor(const CommandsExcutor&& rhs) : _toServerQueue(std::move(rhs._toServerQueue)) {
    }

    CommandsExcutor& CommandsExcutor::operator=(const CommandsExcutor& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = rhs._toServerQueue;
        }
        return *this;
    }

    CommandsExcutor& CommandsExcutor::operator=(const CommandsExcutor&& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = std::move(rhs._toServerQueue);
        }
        return *this;
    }

    bool CommandsExcutor::operator()(const std::string& args) {
        WChat::ChatClient::server::api::ProtoBuffer buff = WChat::ChatClient::server::api::buildRegisterSessionReq(args);
        _toServerQueue->push(buff);
        return true;
    }
};  // namespace WChat::internal::cui
