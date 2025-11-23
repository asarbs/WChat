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

#ifndef COMMANDSEXCUTOR_H
#define COMMANDSEXCUTOR_H

#pragma once

#include <memory>
#include <string>

#include "server/connection/ThreadSafeQueue.h"

namespace WChat::internal::cui {
    class CommandsExcutor {
        public:
            explicit CommandsExcutor(std::shared_ptr<WChat::server::connection::ToWebSockerQueue> toServerQueue);
            virtual ~CommandsExcutor();
            CommandsExcutor(const CommandsExcutor& rhs);
            CommandsExcutor(const CommandsExcutor&& rhs);
            CommandsExcutor& operator=(const CommandsExcutor& rhs);
            CommandsExcutor& operator=(const CommandsExcutor&& rhs);

            bool operator()(const std::string& args);

        protected:
        private:
            std::shared_ptr<WChat::server::connection::ToWebSockerQueue> _toServerQueue;
    };
};  // namespace WChat::internal::cui
#endif
