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

#include "MessageHandler_Message.h"

#include "logger.h"

MessageHandler_Message::MessageHandler_Message() {
}

MessageHandler_Message::~MessageHandler_Message() {
}

void MessageHandler_Message::handle(nlohmann::json::reference payload) {
    logger::logger << logger::debug << "MessageHandler_Message::handle" << logger::endl;
}
