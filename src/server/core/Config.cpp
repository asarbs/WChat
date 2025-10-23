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

#include <string>

namespace WChat::ChatServer::core {
    template <>
    ServerConfig::Config() {
        // Dodanie parametrów domyślnych
        addParam(ParamKey::Host, {"host", "Host of server", "localhost"});
        addParam(ParamKey::Port, {"port", "Port number", "9002"});
    }

    template <>
    const std::string ServerConfig::_confFileName = "WChatServer.conf";

}  // namespace WChat::ChatServer::core

#include "logger.h"
