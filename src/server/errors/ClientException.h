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

#ifndef CLIENTEXCEPTION_H
#define CLIENTEXCEPTION_H

#pragma once

#include <stdexcept>
#include <string>

namespace WChat::ChatServer::errors::client {
    class ClientException : public std::runtime_error {
        public:
            explicit ClientException(const std::string& what_arg) : std::runtime_error(what_arg) {
            }
            explicit ClientException(const char* what_arg) : std::runtime_error(what_arg) {
            }
    };

    class ClientNotRegistered : public ClientException {
        public:
            explicit ClientNotRegistered(const std::string& name) : ClientException("Client with name '" + name + "' is not in DB") {
            }
            explicit ClientNotRegistered(const char* name) : ClientException(std::string("Client with name '") + name + "' is not in DB") {
            }
    };
}  // namespace WChat::ChatServer::errors::client
#endif
