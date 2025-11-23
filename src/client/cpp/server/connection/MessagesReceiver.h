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

#ifndef MESSAGESRECEIVER_H
#define MESSAGESRECEIVER_H

#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include "ChatClient.h"
#include "server/connection/ThreadSafeQueue.h"

namespace WChat::server::connection {

    class MessagesReceiver {
        public:
            MessagesReceiver(std::shared_ptr<WChat::server::connection::FromWebSockerQueue> fromQueue, std::shared_ptr<WChat::ChatClient::ChatClient> client);
            virtual ~MessagesReceiver();
            MessagesReceiver(const MessagesReceiver& rhs)             = delete;
            MessagesReceiver(const MessagesReceiver&& rhs)            = delete;
            MessagesReceiver& operator=(const MessagesReceiver& rhs)  = delete;
            MessagesReceiver& operator=(const MessagesReceiver&& rhs) = delete;
            void start();
            void stop();

        protected:
            //
        private:
            void _run();
            std::thread _thread;
            std::atomic<bool> _running = false;

            std::shared_ptr<WChat::server::connection::FromWebSockerQueue> _fromQueue;
            std::shared_ptr<WChat::ChatClient::ChatClient> _client;

            void _handelRegisterSessionRes(const WChat::RegisterSessionRes& m);
    };
};  // namespace WChat::server::connection
#endif
