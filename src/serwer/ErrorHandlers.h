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

#ifndef ERRORHANDLERS_H
#define ERRORHANDLERS_H

#pragma once

#include <stdexcept>
#include <string>

namespace wchat::protocul {
    // class ErrorHandlers {
    //     public:
    //         ErrorHandlers();
    //         ~ErrorHandlers();

    //     protected:
    //     private:
    // };

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
}  // namespace wchat::protocul

#endif
