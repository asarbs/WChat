/*
 * World VTT
 *
 * Copyright (C) 2024, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 *
 */

#include "ChatClientDatabase.h"

#include "logger.h"

ChatClientDatabase& ChatClientDatabase::getInstance() {
    static ChatClientDatabase instance;
    return instance;
}

// ChatClientDatabase::ChatClientDatabase() {
// }

// ChatClientDatabase::~ChatClientDatabase() {
// }

void ChatClientDatabase::regiserClinet(websocketpp::connection_hdl hdl) {
    ChatClient cc;
    cc.connection       = hdl;
    __chat_clients[hdl] = cc;
    logger::logger << logger::debug << "ChatClientDatabase::regiserClinet" << logger::endl;
}
