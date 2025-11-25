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

#include "Config.h"

#include <memory>
#include <string>
#include <vector>

namespace WChat::ChatServer::core {

#ifdef TARGET_NAME_CLIENT
    template <>
    ServerConfig::Config() {
        addParam(ParamKey::Host, std::make_shared<ConfigParameter>("host", "Host of server", "localhost"));
        addParam(ParamKey::Port, std::make_shared<ConfigParameter>("port", "Port number", "9002"));
        addParam(ParamKey::UserName, std::make_shared<ConfigParameter>("userame", "Client User name", "username"));
    }
    template <>
    const std::string ServerConfig::_confFileName = "WChatCliet.conf";
#else
    template <>
    ServerConfig::Config() {
        // Dodanie parametrów domyślnych
        addParam(ParamKey::Host, std::make_shared<ConfigParameter>("host", "Host of server", "localhost"));
        addParam(ParamKey::Port, std::make_shared<ConfigParameter>("port", "Port number", "9002"));
        addParam(ParamKey::Storage, std::make_shared<ConfigChoice>("Storage", "Storage of serwer data", "volatile", std::vector<std::string>{"volatile", "db"}));
    }

    template <>
    const std::string ServerConfig::_confFileName = "WChatServer.conf";
#endif

}  // namespace WChat::ChatServer::core

#include "logger.h"
