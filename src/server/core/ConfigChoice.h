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

#ifndef CONFIGCHOICE_H
#define CONFIGCHOICE_H

#pragma once

#include <string>
#include <vector>

#include "server/core/ConfigParameter.h"

namespace WChat::ChatServer::core {
    class ConfigChoice : public ConfigParameter {
        public:
            ConfigChoice();
            ConfigChoice(const std::string& name, const std::string& description, const std::string& value, const std::vector<std::string>& choice);
            ~ConfigChoice() override;
            ConfigChoice(const ConfigChoice& rhs);
            ConfigChoice(const ConfigChoice&& rhs);
            ConfigChoice& operator=(const ConfigChoice& rhs);
            ConfigChoice& operator=(const ConfigChoice&& rhs);

        protected:
            //
        private:
            std::vector<std::string> _choice;
    };
}  // namespace WChat::ChatServer::core

#endif
