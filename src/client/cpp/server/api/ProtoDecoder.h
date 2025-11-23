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

#ifndef PROTODECODER_H
#define PROTODECODER_H

#pragma once

#include <memory>
#include <queue>
#include <string>
#include <variant>

#include "proto/messeges.pb.h"
#include "server/connection/ThreadSafeQueue.h"

namespace WChat::ChatClient::server::api {
    class ProtoDecoder {
        public:
            explicit ProtoDecoder(std::shared_ptr<WChat::server::connection::FromWebSockerQueue> fromSeverQueue);
            virtual ~ProtoDecoder();
            ProtoDecoder(const ProtoDecoder& rhs);
            ProtoDecoder(const ProtoDecoder&& rhs);
            ProtoDecoder& operator=(const ProtoDecoder& rhs);
            ProtoDecoder& operator=(const ProtoDecoder&& rhs);

            void decode(const std::string& payload);

        protected:
        private:
            std::shared_ptr<WChat::server::connection::FromWebSockerQueue> _fromSeverQueue;
    };
};  // namespace WChat::ChatClient::server::api
#endif
