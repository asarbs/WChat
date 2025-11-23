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

#include "MessagesReceiver.h"

#include "logger.h"
namespace WChat::server::connection {

    MessagesReceiver::MessagesReceiver(std::shared_ptr<WChat::server::connection::FromWebSockerQueue> fromQueue, std::shared_ptr<WChat::ChatClient::ChatClient> client)
        : _fromQueue(fromQueue), _client(client) {
    }

    MessagesReceiver::~MessagesReceiver() {
        stop();
    }

    void MessagesReceiver::start() {
        if (_running) {
            return;
        }
        _running = true;
        _thread  = std::thread(&MessagesReceiver::_run, this);
        logger::logger << logger::debug << "MessagesReceiver::start" << logger::endl;
    }

    void MessagesReceiver::stop() {
        _running = false;
        logger::logger << logger::debug << "MessagesReceiver::stop" << logger::endl;
    }

    void MessagesReceiver::_run() {
        while (_running) {
            logger::logger << logger::debug << "Waiting for message..." << logger::endl;

            WChat::ChatClient::server::api::ProtoRxMessages msg = _fromQueue->waitAndPop();

            if (std::holds_alternative<WChat::Msg>(msg)) {
                // const WChat::Msg& m = std::get<WChat::Msg>(msg);  //
                logger::logger << logger::debug << "Got Msg" << logger::endl;
            } else if (std::holds_alternative<WChat::RegisterSessionRes>(msg)) {
                const WChat::RegisterSessionRes& m = std::get<WChat::RegisterSessionRes>(msg);
                _handelRegisterSessionRes(m);
            } else if (std::holds_alternative<WChat::TextMessage>(msg)) {
                // const WChat::TextMessage& m = std::get<WChat::TextMessage>(msg);
                logger::logger << logger::debug << "Got TextMessage" << logger::endl;
            } else if (std::holds_alternative<WChat::ListContactRes>(msg)) {
                // const WChat::ListContactRes& m = std::get<WChat::ListContactRes>(msg);
                logger::logger << logger::debug << "Got ListContactRes" << logger::endl;
            } else {
                logger::logger << logger::error << "Code should not be here" << logger::endl;
            }
        }
    }
    void MessagesReceiver::_handelRegisterSessionRes(const WChat::RegisterSessionRes& m) {
        logger::logger << logger::debug << "handel: RegisterSessionRes" << logger::endl;
        _client->setUserId(m.user_id());
    }

};  // namespace WChat::server::connection
