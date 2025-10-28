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

#ifndef CONFIG_H
#define CONFIG_H

#pragma once

#include <exception>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "logger.h"
#include "server/core/ConfigChoice.h"
#include "server/core/ConfigParameter.h"

namespace WChat::ChatServer::core {
    enum class ParamKey {
        Host,
        Port,
        Storage
    };
    template <class ParamsDict>
    class Config {
        public:
            class ParameterAlreadyRegistered : std::exception {};
            class ConfigurationFileError : std::exception {};
            static Config<ParamsDict>& instance() {  // cppcheck-suppress unusedFunction
                static Config<ParamsDict> instance;
                return instance;
            }
            void addParam(const ParamsDict& key, std::shared_ptr<ConfigParameter> param) {  // cppcheck-suppress unusedFunction
                // if (_paramCollection.find(key) != _paramCollection.end() || _params2enums.find(param.name()) != _params2enums.end()) {
                if (_paramCollection.find(key) != _paramCollection.end()) {
                    throw ParameterAlreadyRegistered();
                }
                _paramCollection[key]        = param;
                _params2enums[param->name()] = key;
            }

            const std::shared_ptr<ConfigParameter> get(const ParamsDict& key) const {
                return _paramCollection.at(key);
            }

            template <typename T>
            T value(const ParamsDict& key) const {
                return get(key)->template as<T>();
            }

            void saveToFile() {  // cppcheck-suppress unusedFunction
                std::ofstream out(_confFileName);

                for (const auto& [keys, params] : _paramCollection) {
                    out << "# " << params->description() << "\n" << params->name() << "=" << params->value() << "\n";
                }
                out.close();
            }

            void loadFromFile() {
                // logger::logger << logger::debug << "Open configuration file: " << _confFileName << logger::endl;
                std::ifstream in(_confFileName);
                if (!in.is_open()) {
                    throw ConfigurationFileError();
                }
                std::string line;
                while (std::getline(in, line)) {
                    if (line[0] == '#') {
                        // logger::logger << logger::debug << "Commnet: '" << line << "'." << logger::endl;
                    } else {
                        std::pair<std::string, std::string> parameter = splitParam(line, '=');
                        // logger::logger << logger::debug << "Param: '" << parameter.first << "->" << parameter.second << "'." << logger::endl;
                        ParamsDict key                                = _params2enums[parameter.first];
                        _paramCollection.at(key)->set(parameter.second);
                    }
                }
                in.close();
            }

            void clear() {
                _paramCollection.clear();
            }

        protected:
            //
        private:
            Config() {
            }
            ~Config() {
            }
            std::map<ParamsDict, std::shared_ptr<ConfigParameter>> _paramCollection;
            std::map<std::string, ParamsDict> _params2enums;
            static const std::string _confFileName;

            std::pair<std::string, std::string> splitParam(const std::string& in, char delimiter) {
                size_t delimiterPos = in.find_first_of(delimiter);
                return {in.substr(0, delimiterPos), in.substr(delimiterPos + 1)};
            }
    };

    using ServerConfig = Config<ParamKey>;
};  // namespace WChat::ChatServer::core
#endif
