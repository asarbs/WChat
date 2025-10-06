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

#ifndef MESSAGEHANDLER_MESSAGE_H
#define MESSAGEHANDLER_MESSAGE_H

#pragma once

#include "serwer/ChatClient.h"
#include "serwer/ChatClientDatabase.h"
#include "serwer/messages/MessageHandler.h"

class MessageHandler_Message : public MessageHandler {
    public:
        MessageHandler_Message();
        ~MessageHandler_Message() override;
        void handle(server* s, const websocketpp::connection_hdl& hdl, nlohmann::json::reference payload) override;

    protected:
    private:
};

#endif
