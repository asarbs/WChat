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

namespace WChat::ChatClient {
    class ChatClient {
        public:
            ChatClient();
            virtual ~ChatClient();
            ChatClient(const ChatClient& rhs);
            ChatClient(const ChatClient&& rhs);
            ChatClient& operator=(const ChatClient& rhs);
            ChatClient& operator=(const ChatClient&& rhs);

        protected:
        private:
    };
};  // namespace WChat::ChatClient
#endif
