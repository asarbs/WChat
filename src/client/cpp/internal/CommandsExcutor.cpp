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

#include "ServerApi/ProtoWrapper.h"
#include "logger.h"

namespace WChat::internal::cui {
    CommandsExcutor::CommandsExcutor(std::shared_ptr<ToWebSockerQueue> toServerQueue) : _toServerQueue(toServerQueue) {
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
        WChat::ChatClient::ServerAPI::ProtoBuffer buff = WChat::ChatClient::ServerAPI::buildRegisterSessionReq(args);
        _toServerQueue->push(buff);
        return true;
    }
};  // namespace WChat::internal::cui
