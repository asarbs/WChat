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

#include "Config.h"
#include "ConfigChoice.h"
#include "ConfigParameter.h"
#include "logger.h"

namespace WChat::internal::Config {

    enum class ParamKey {
        Host,
        Port,
        UserName,
    };

    using ClientConfig = cpp_config::Config<ParamKey>;

}  // namespace WChat::internal::Config

namespace cpp_config {

    template <>
    inline Config<WChat::internal::Config::ParamKey>::Config() {
        using PK = WChat::internal::Config::ParamKey;

        addParam(PK::Host, std::make_shared<ConfigParameter>("host", "Host of server", "localhost"));
        addParam(PK::Port, std::make_shared<ConfigParameter>("port", "Port number", "9002"));
        addParam(PK::UserName, std::make_shared<ConfigParameter>("userame", "Client User name", "username"));
    }

    template <>
    inline const std::string Config<WChat::internal::Config::ParamKey>::_confFileName = "WChatServer.conf";

}  // namespace cpp_config
#endif
