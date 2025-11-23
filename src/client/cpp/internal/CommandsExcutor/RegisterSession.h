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

#ifndef COMMANDSEXCUTOR_H
#define COMMANDSEXCUTOR_H

#pragma once

#include <memory>
#include <string>

#include "ChatClient.h"
#include "server/connection/ThreadSafeQueue.h"

namespace WChat::internal::cui::excutor {
    class RegisterSession {
        public:
            explicit RegisterSession(std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toServerQueue, std::shared_ptr<WChat::ChatClient::ChatClient> client);
            virtual ~RegisterSession();
            RegisterSession(const RegisterSession& rhs);
            RegisterSession(const RegisterSession&& rhs);
            RegisterSession& operator=(const RegisterSession& rhs);
            RegisterSession& operator=(const RegisterSession&& rhs);

            bool operator()(const std::string& args);

        protected:
        private:
            std::shared_ptr<WChat::server::connection::ToWebSockerQueue> _toServerQueue;
            std::shared_ptr<WChat::ChatClient::ChatClient> _client;
    };
};  // namespace WChat::internal::cui::excutor
#endif
