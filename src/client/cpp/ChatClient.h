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

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <cstdint>
#include <string>

namespace WChat::ChatClient {
    class ChatClient {
        public:
            ChatClient();
            virtual ~ChatClient();
            ChatClient(const ChatClient& rhs);
            ChatClient(const ChatClient&& rhs);
            ChatClient& operator=(const ChatClient& rhs);
            ChatClient& operator=(const ChatClient&& rhs);

            void setUserId(uint64_t userId);
            uint64_t getUserId() const;
            void setName(const std::string& name);

        protected:
        private:
            uint64_t _userId;
            std::string _name;
    };
};  // namespace WChat::ChatClient
#endif
