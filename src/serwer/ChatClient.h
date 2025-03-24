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

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <websocketpp/server.hpp>

struct ChatClient: websocketpp::connection_hdl
{
    public:
        ChatClient();
        ~ChatClient();
        websocketpp::connection_hdl connection;
        std::string user_id;

    protected:


    private:
};

#endif
