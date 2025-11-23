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

#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

#include "server/api/ProtoWrapper.h"

namespace WChat::server::connection {

    template <typename T>
    class ThreadSafeQueue {
        public:
            ThreadSafeQueue()                                       = default;
            virtual ~ThreadSafeQueue()                              = default;
            ThreadSafeQueue(const ThreadSafeQueue& rhs)             = delete;
            ThreadSafeQueue(const ThreadSafeQueue&& rhs)            = delete;
            ThreadSafeQueue& operator=(const ThreadSafeQueue& rhs)  = delete;
            ThreadSafeQueue& operator=(const ThreadSafeQueue&& rhs) = delete;
            void push(T val) {
                {
                    std::lock_guard<std::mutex> lock(_mutex);
                    _queue.push(std::move(val));
                }
                _cv.notify_one();
            }

            T waitAndPop() {  // cppcheck-suppress unusedFunction
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this] { return !_queue.empty(); });
                T value = std::move(_queue.front());
                _queue.pop();
                return value;
            }

            std::optional<T> tryPop() {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_queue.empty()) {
                    return {};
                }
                T value = std::move(_queue.front());
                _queue.pop();
                return value;
            }

        protected:
            //
        private:
            std::queue<T> _queue;
            std::mutex _mutex;
            std::condition_variable _cv;
    };

    using ToWebSockerQueue = ThreadSafeQueue<WChat::ChatClient::server::api::ProtoBuffer>;

    using FromWebSockerQueue = ThreadSafeQueue<WChat::ChatClient::server::api::ProtoRxMessages>;
};  // namespace WChat::server::connection
#endif
