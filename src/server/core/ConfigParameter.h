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

#ifndef CONFIGPARAMETER_H
#define CONFIGPARAMETER_H

#pragma once

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "logger.h"

namespace WChat::ChatServer::core {

    class ConfigParameter {
        public:
            ConfigParameter() : _name("name"), _description(""), _value("") {
            }

            ConfigParameter(std::string name, std::string description, const std::string& value) : _name(name), _description(description), _value(value) {
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
            const std::string& value() const {
                return _value;
            }

            void set(const std::string& val) {
                _value = val;
            }

            template <typename T>
            T as() const {
                if constexpr (std::is_same_v<T, std::string>) {
                    return _value;
                } else if constexpr (std::is_same_v<T, bool>) {
                    return "true" == _value;
                } else if constexpr (std::is_integral_v<T>) {
                    return static_cast<T>(std::stoll(_value));
                } else if constexpr (std::is_floating_point_v<T>) {
                    return static_cast<T>(std::stod(_value));
                } else {
                    throw std::invalid_argument("Unsupported type.");
                }
            }

        protected:
            //
        private:
            std::string _name;
            std::string _description;
            std::string _value;
    };
}  // namespace WChat::ChatServer::core
#endif
