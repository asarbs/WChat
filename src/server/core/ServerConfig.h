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

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#pragma once

#include <exception>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Config.h"
#include "ConfigChoice.h"
#include "ConfigParameter.h"
#include "logger.h"

namespace WChat::ChatServer::core {

    enum class ParamKey {
        Host,
        Port,
        Storage,
        UserName,
    };

    using ServerConfig = cpp_config::Config<ParamKey>;

}  // namespace WChat::ChatServer::core

namespace cpp_config {

    template <>
    inline Config<WChat::ChatServer::core::ParamKey>::Config() {
        using PK = WChat::ChatServer::core::ParamKey;

        addParam(PK::Host, std::make_shared<ConfigParameter>("host", "Host of server", "localhost"));
        addParam(PK::Port, std::make_shared<ConfigParameter>("port", "Port number", "9002"));
        addParam(PK::Storage, std::make_shared<ConfigChoice>("Storage", "Storage of server data", "volatile", std::vector<std::string>{"volatile", "db"}));
    }

    template <>
    inline const std::string Config<WChat::ChatServer::core::ParamKey>::_confFileName = "WChatServer.conf";

}  // namespace cpp_config
#endif
