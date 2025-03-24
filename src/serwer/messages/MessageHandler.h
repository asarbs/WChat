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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#pragma once

#include <nlohmann/json.hpp>

class MessageHandler
{
    public:
        MessageHandler();
        ~MessageHandler() = default;
        virtual void handle(nlohmann::json::reference payload) = 0;

protected:

private:
};

#endif
