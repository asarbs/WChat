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

#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once

#include <string>

#include "server/api/ProtoWrapper.h"

namespace WChat::ChatClient::internal {
    struct Message {
            enum class Direction {
                Tx,  // in direction to Chat Server
                Rx,  // in direction from Chat Server
            };
            Direction dir;
            WChat::ChatClient::server::api::ProtoBuffer data;
    };
};  // namespace WChat::ChatClient::internal
#endif
