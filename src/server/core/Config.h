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

#ifndef CONFIG_H
#define CONFIG_H

#pragma once

#include <exception>
#include <map>

#include "server/core/ConfigParameter.h"

namespace WChat::ChatServer::core {
    template <class ParamsDict>
    class Config {
        public:
            class ParameterAlreadyRegistered : std::exception {};
            static Config<ParamsDict>& instance() {  // cppcheck-suppress unusedFunction
                static Config<ParamsDict> instance;
                return instance;
            }
            void addParam(const ParamsDict& key, ConfigParameter param) {  // cppcheck-suppress unusedFunction
                if (_param_collection.find(key) != _param_collection.end()) {
                    throw ParameterAlreadyRegistered();
                }
                _param_collection[key] = param;
            }

            const ConfigParameter& get(const ParamsDict& key) {
                return _param_collection[key];
            }

            template <typename T>
            T value(const ParamsDict& key) {
                return std::get<T>(get(key).value());
            }

            void clear() {
                _param_collection.clear();
            }

        protected:
            //
        private:
            Config() {
            }
            ~Config() {
            }
            std::map<ParamsDict, ConfigParameter> _param_collection;
    };
};  // namespace WChat::ChatServer::core
#endif
