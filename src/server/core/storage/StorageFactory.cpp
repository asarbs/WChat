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

#include "StorageFactory.h"

#include <string>

#include "logger.h"
#include "server/core/storage/db/sqlite/SQLightWrapper.h"
#include "server/errors/ErrorHandlers.h"

namespace WChat::ChatServer::core::storage {
    std::shared_ptr<Storage> getStorage() {
        std::string storage = WChat::ChatServer::core::ServerConfig::instance().value<std::string>(WChat::ChatServer::core::ParamKey::Storage);
        if (storage == "db") {
            return std::shared_ptr<Storage>(&WChat::ChatServer::core::storage::db::sqlite::SQLightWrapper::instance(), [](Storage*) {});  // empty deleter
        } else {
            std::stringstream ss;
            ss << "Unsupported data storage method `" << storage << "`." << std::endl;
            throw WChat::ChatServer::errors::ConfigurationError(ss.str());
        }
    }
};  // namespace WChat::ChatServer::core::storage
