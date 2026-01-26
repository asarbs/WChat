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

#include "Contacts.h"

#include <iostream>
#include <unordered_map>

#include "logger.h"

namespace WChat::internal::cui::excutor {

    Contacts::Contacts(std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toServerQueue, std::shared_ptr<WChat::ChatClient::ChatClient> client)
        : _toServerQueue(toServerQueue), _client(client) {
    }

    Contacts::~Contacts() {
    }

    Contacts::Contacts(const Contacts& rhs) : _toServerQueue(rhs._toServerQueue), _client(rhs._client) {
    }

    Contacts::Contacts(const Contacts&& rhs) : _toServerQueue(std::move(rhs._toServerQueue)), _client(std::move(rhs._client)) {
    }

    Contacts& Contacts::operator=(const Contacts& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = rhs._toServerQueue;
            this->_client        = rhs._client;
        }
        return *this;
    }

    Contacts& Contacts::operator=(const Contacts&& rhs) {
        if (this != &rhs) {
            this->_toServerQueue = rhs._toServerQueue;
            this->_client        = rhs._client;
        }
        return *this;
    }

    bool Contacts::operator()(const std::string& args) {
        if (_client == NULL) {
            logger::logger << logger::error << "Can't execute command! _client is NULL" << logger::endl;
            return false;
        }
        const auto& contacts = _client->getContacts();
        for (const auto& [name, id] : contacts) {
            std::cout << name << std::endl;
        }
        return true;
    }
};  // namespace WChat::internal::cui::excutor
