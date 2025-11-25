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

#include "ChatClient.h"
#include "internal/CUI.h"
#include "internal/CommandsExcutor/RegisterSession.h"
#include "internal/CommandsExcutor/SendMsg.h"
#include "logger.h"
#include "server/api/ProtoDecoder.h"
#include "server/api/ProtoWrapper.h"
#include "server/connection/MessagesReceiver.h"
#include "server/connection/WebSocketWorker.h"
int main() {
    std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toQueue     = std::make_shared<WChat::server::connection::ToWebSockerQueue>();
    std::shared_ptr<WChat::server::connection::FromWebSockerQueue> fromQueue = std::make_shared<WChat::server::connection::FromWebSockerQueue>();
    std::shared_ptr<WChat::ChatClient::ChatClient> client                    = std::make_shared<WChat::ChatClient::ChatClient>();
    WChat::server::connection::WebSocketWorker sockertWorker(toQueue, fromQueue);
    WChat::internal::cui::CUIWorker cui;

    sockertWorker.start();
    cui.registerCommand("reg", WChat::internal::cui::excutor::RegisterSession{toQueue, client});
    cui.registerCommand("msg", WChat::internal::cui::excutor::SendMsg{toQueue, client});
    cui.start();

    WChat::server::connection::MessagesReceiver receiver(fromQueue, client);
    receiver.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    receiver.stop();
    cui.stop();
    sockertWorker.stop();

    return 0;
}
