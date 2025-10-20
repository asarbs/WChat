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

#ifndef CONFIGPARAMETER_H
#define CONFIGPARAMETER_H

#pragma once

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace WChat::ChatServer::core {
    using ConfigValue = std::variant<uint32_t, int32_t, double, std::string, bool>;

    class ConfigParameter {
        public:
            ConfigParameter() : _name("name"), _description(""), _value(0) {
            }

            ConfigParameter(std::string name, std::string description, ConfigValue value) : _name(name), _description(description), _value(value) {
                if (_name.find(' ') != std::string::npos) {
                    throw std::invalid_argument("ConfigParameter name cannot contain spaces");
                }
            }

            ConfigParameter(ConfigParameter& rhs) : _name(rhs._name), _description(rhs._description), _value(rhs._value) {
            }

            ConfigParameter(ConfigParameter&& rhs) : _name(std::move(rhs._name)), _description(std::move(rhs._description)), _value(std::move(rhs._value)) {
            }

            ConfigParameter& operator=(const ConfigParameter& rhs) {
                _name        = rhs._name;
                _description = rhs._description;
                _value       = rhs._value;
                return *this;
            }

            ConfigParameter& operator=(const ConfigParameter&& rhs) {
                _name        = rhs._name;
                _description = rhs._description;
                _value       = rhs._value;
                return *this;
            }

            const std::string& name() const {  // cppcheck-suppress unusedFunction
                return _name;
            }
            const std::string& description() const {  // cppcheck-suppress unusedFunction
                return _description;
            }
            ConfigValue value() const {
                return _value;
            }

            std::string toString() const {
                return std::visit(
                    [](auto&& arg) -> std::string {
                        std::stringstream ss;
                        ss << arg;
                        return ss.str();
                    },
                    _value);
            }

        protected:
            //
        private:
            std::string _name;
            std::string _description;
            ConfigValue _value;
    };
}  // namespace WChat::ChatServer::core
#endif
