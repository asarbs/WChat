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

#ifndef WEBSOCKETWORKER_H
#define WEBSOCKETWORKER_H

#pragma once

#include <ixwebsocket/IXWebSocket.h>

#include <atomic>
#include <memory>
#include <thread>

#include "server/api/ProtoDecoder.h"
#include "server/connection/ThreadSafeQueue.h"

namespace WChat::server::connection {
    class WebSocketWorker {
        public:
            WebSocketWorker(std::shared_ptr<ToWebSockerQueue> toServerQueue, std::shared_ptr<FromWebSockerQueue> fromServerQueue);
            virtual ~WebSocketWorker();
            WebSocketWorker(const WebSocketWorker& rhs)             = delete;
            WebSocketWorker(const WebSocketWorker&& rhs)            = delete;
            WebSocketWorker& operator=(const WebSocketWorker& rhs)  = delete;
            WebSocketWorker& operator=(const WebSocketWorker&& rhs) = delete;
            void start();
            void stop();

        protected:
        private:
            void run();

            std::thread _workerThread;
            std::atomic<bool> _running = false;
            std::atomic<bool> _isOpen  = false;
            WChat::ChatClient::server::api::ProtoDecoder _decoder;

            ix::WebSocket _webSocket;

            std::shared_ptr<ToWebSockerQueue> _toServerQueue;
            std::shared_ptr<FromWebSockerQueue> _fromSeverQueue;
    };
};  // namespace WChat::server::connection
#endif
