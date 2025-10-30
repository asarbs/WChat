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

#ifndef STORAGEFACTORY_H
#define STORAGEFACTORY_H

#include <memory>

#include "server/core/Config.h"
#include "server/core/storage/Storage.h"

#pragma once
namespace WChat::ChatServer::core::storage {
    std::shared_ptr<Storage> getStorage();
};  // namespace WChat::ChatServer::core::storage

#endif
