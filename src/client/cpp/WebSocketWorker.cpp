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

#include "WebSocketWorker.h"

#include <memory>

#include "logger.h"

WebSocketWorker::WebSocketWorker(std::shared_ptr<ToWebSockerQueue> toServerQueue, std::shared_ptr<FromWebSockerQueue> fromServerQueue)
    : _toServerQueue(toServerQueue), _fromSeverQueue(fromServerQueue), _decoder(fromServerQueue) {
    _webSocket.setUrl("ws://localhost:9002/ws");
    _webSocket.disablePerMessageDeflate();
    _webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            logger::logger << logger::debug << "Open Connection" << logger::endl;
            _isOpen = true;
            // Headers can be inspected (pairs of string/string)
        } else if (msg->type == ix::WebSocketMessageType::Message) {
            _decoder.decode(msg->str);
        } else if (msg->type == ix::WebSocketMessageType::Close) {
            logger::logger << logger::debug << "Close Connection" << logger::endl;

            // The server can send an explicit code and reason for closing.
            // This data can be accessed through the closeInfo object.
            logger::logger << logger::debug << msg->closeInfo.code << logger::endl;
            logger::logger << logger::debug << msg->closeInfo.reason << logger::endl;
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            std::stringstream ss;
            ss << "Error: " << msg->errorInfo.reason << "\n";
            ss << "#retries: " << msg->errorInfo.retries << "\n";
            ss << "Wait time(ms): " << msg->errorInfo.wait_time << "\n";
            ss << "HTTP Status: " << msg->errorInfo.http_status << "\n";
            logger::logger << logger::debug << ss.str() << logger::endl;
        } else if (msg->type == ix::WebSocketMessageType::Ping || msg->type == ix::WebSocketMessageType::Pong) {
            logger::logger << logger::debug << "pong data: " << msg->str << logger::endl;
        }
    });
}

WebSocketWorker::~WebSocketWorker() {
    stop();
}

void WebSocketWorker::start() {
    if (_running) {
        return;
    }
    _running      = true;
    _workerThread = std::thread(&WebSocketWorker::run, this);
}

void WebSocketWorker::stop() {
    _running = false;
}

void WebSocketWorker::run() {
    _webSocket.start();

    while (_running) {
        if (auto msg = _toServerQueue->tryPop()) {
            logger::logger << logger::debug << "Send msg to webSocket" << logger::endl;
            _webSocket.sendBinary(msg.value());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
