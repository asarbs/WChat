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

#ifndef SENDMSG_H
#define SENDMSG_H

#pragma once

#include <memory>
#include <string>

#include "ChatClient.h"
#include "server/connection/ThreadSafeQueue.h"

namespace WChat::internal::cui::excutor {
    class SendMsg {
        public:
            SendMsg(std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toServerQueue, std::shared_ptr<WChat::ChatClient::ChatClient> client);
            virtual ~SendMsg();
            SendMsg(const SendMsg& rhs);
            SendMsg(const SendMsg&& rhs);
            SendMsg& operator=(const SendMsg& rhs);
            SendMsg& operator=(const SendMsg&& rhs);

            bool operator()(const std::string& args);

        protected:
        private:
            std::shared_ptr<WChat::server::connection::ToWebSockerQueue> _toServerQueue;
            std::shared_ptr<WChat::ChatClient::ChatClient> _client;
    };
};  // namespace WChat::internal::cui::excutor
#endif
