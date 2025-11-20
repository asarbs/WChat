/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 */

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ServerApi/ProtoDecoder.h"
#include "ServerApi/ProtoWrapper.h"
#include "WebSocketWorker.h"
#include "internal/CUI.h"
#include "internal/CommandsExcutor.h"
#include "logger.h"

int main() {
    std::shared_ptr<ToWebSockerQueue> toQueue     = std::make_shared<ToWebSockerQueue>();
    std::shared_ptr<FromWebSockerQueue> fromQueue = std::make_shared<FromWebSockerQueue>();

    WebSocketWorker sockertWorker(toQueue, fromQueue);
    sockertWorker.start();
    WChat::internal::cui::CUIWorker cui;
    cui.registerCommand("example", WChat::internal::cui::exampleCUIFunction);
    cui.registerCommand("reg", WChat::internal::cui::CommandsExcutor{toQueue});
    cui.start();

    std::thread receiver([fromQueue] {
        logger::logger << logger::debug << "Start of receiver thread" << logger::endl;
        while (true) {
            logger::logger << logger::debug << "Waiting for message..." << logger::endl;

            WChat::ChatClient::ServerAPI::ProtoRxMessages msg = fromQueue->waitAndPop();

            if (std::holds_alternative<WChat::Msg>(msg)) {
                const WChat::Msg& m = std::get<WChat::Msg>(msg);
                logger::logger << logger::debug << "Got Msg" << logger::endl;
            } else if (std::holds_alternative<WChat::RegisterSessionRes>(msg)) {
                const WChat::RegisterSessionRes& m = std::get<WChat::RegisterSessionRes>(msg);
                logger::logger << logger::debug << "Got RegisterSessionRes" << logger::endl;
            } else if (std::holds_alternative<WChat::TextMessage>(msg)) {
                const WChat::TextMessage& m = std::get<WChat::TextMessage>(msg);
                logger::logger << logger::debug << "Got TextMessage" << logger::endl;
            } else if (std::holds_alternative<WChat::ListContactRes>(msg)) {
                const WChat::ListContactRes& m = std::get<WChat::ListContactRes>(msg);
                logger::logger << logger::debug << "Got ListContactRes" << logger::endl;
            } else {
                logger::logger << logger::error << "Code should not be here" << logger::endl;
            }
        }
    });

    receiver.join();
    cui.stop();
    sockertWorker.stop();

    return 0;
}
