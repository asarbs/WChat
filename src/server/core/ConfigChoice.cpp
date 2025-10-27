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

#include "ConfigChoice.h"

#include <utility>

#include "logger.h"

namespace WChat::ChatServer::core {
    ConfigChoice::ConfigChoice() : ConfigParameter() {
    }
    ConfigChoice::ConfigChoice(const std::string& name, const std::string& description, const std::string& value, const std::vector<std::string>& choice)
        : ConfigParameter(name, description, value), _choice(choice) {
    }
    ConfigChoice::~ConfigChoice() {
    }
    ConfigChoice::ConfigChoice(const ConfigChoice& rhs) : ConfigParameter(rhs), _choice(rhs._choice) {
    }
    ConfigChoice::ConfigChoice(const ConfigChoice&& rhs) : ConfigParameter(std::move(rhs)), _choice(std::move(rhs._choice)) {
    }
    ConfigChoice& ConfigChoice::operator=(const ConfigChoice& rhs) {
        if (this != &rhs) {
            ConfigParameter::operator=(rhs);
            _choice = rhs._choice;
        }
        return *this;
    }
    ConfigChoice& ConfigChoice::operator=(const ConfigChoice&& rhs) {
        if (this != &rhs) {
            ConfigParameter::operator=(std::move(rhs));
            _choice = std::move(rhs._choice);
        }
        return *this;
    }

}  // namespace WChat::ChatServer::core
