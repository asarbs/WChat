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

#include "CUI.h"

#include "logger.h"

namespace WChat::internal::cui {

    CUIWorker::CUIWorker() : _running(false) {
    }

    CUIWorker::~CUIWorker() {
    }

    void CUIWorker::registerCommand(const std::string& name, CommadFun fun) {
        _cmdRegister[name] = fun;
    }
    void CUIWorker::start() {
        _running = true;
        logger::logger << logger::debug << "CUIWorker::start" << logger::endl;
        _worker = std::thread(&CUIWorker::_run, this);
    }
    void CUIWorker::stop() {
        _running = false;
        if (_worker.joinable()) {
            _worker.join();
            logger::logger << logger::debug << "CUIWorker::stop" << logger::endl;
        }
    }
    void CUIWorker::_run() {
        std::string line;

        while (_running) {
            std::cout << "> " << std::flush;

            if (!std::getline(std::cin, line)) {
                _running = false;
                break;
            }

            if (line.empty()) {
                continue;
            }

            std::istringstream iss(line);
            std::string cmd;
            iss >> cmd;

            std::string args;
            std::getline(iss, args);
            if (!args.empty() && args[0] == ' ') {
                args.erase(0, 1);
            }

            auto it = _cmdRegister.find(cmd);
            if (it == _cmdRegister.end()) {
                std::cout << "Unknown command: `" << cmd << "`" << std::endl;
                continue;
            }
            it->second(args);
        }
    }

    bool exampleCUIFunction(const std::string& args) {
        std::cout << "Example CUI Function with args=`" << args << "`" << std::endl;
        return true;
    }

};  // namespace WChat::internal::cui
