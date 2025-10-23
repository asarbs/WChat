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

#ifndef ERRORHANDLERS_H
#define ERRORHANDLERS_H

#pragma once

#include <stdexcept>
#include <string>

#define THROW_NOT_IMPLEMENTED()                                                                    \
    do {                                                                                           \
        std::ostringstream oss;                                                                    \
        oss << "Unimplemented yet! (" << __FILE__ << ":" << __LINE__ << " in " << __func__ << ")"; \
        throw std::runtime_error(oss.str());                                                       \
    } while (0)

namespace WChat::ChatServer::errors {
    class ProtoculError : public std::runtime_error {
        public:
            explicit ProtoculError(const std::string& what_arg) : std::runtime_error(what_arg) {
            }
            explicit ProtoculError(const char* what_arg) : std::runtime_error(what_arg) {
            }
    };
    class ProtoculWarning : public std::runtime_error {
        public:
            explicit ProtoculWarning(const std::string& what_arg) : std::runtime_error(what_arg) {
            }
            explicit ProtoculWarning(const char* what_arg) : std::runtime_error(what_arg) {
            }
    };
};  // namespace WChat::ChatServer::errors
#endif
