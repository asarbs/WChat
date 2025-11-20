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

#ifndef CUI_H
#define CUI_H

#include <atomic>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

#pragma once
namespace WChat::internal::cui {
    using CommadFun = std::function<bool(const std::string&)>;
    class CUIWorker {
        public:
            CUIWorker();
            virtual ~CUIWorker();
            CUIWorker(const CUIWorker& rhs)             = delete;
            CUIWorker(const CUIWorker&& rhs)            = delete;
            CUIWorker& operator=(const CUIWorker& rhs)  = delete;
            CUIWorker& operator=(const CUIWorker&& rhs) = delete;
            void registerCommand(const std::string& name, CommadFun fun);
            void start();
            void stop();

        protected:
            //
        private:
            void _run();
            std::atomic<bool> _running;
            std::unordered_map<std::string, CommadFun> _cmdRegister;
            std::thread _worker;
    };

    bool exampleCUIFunction(const std::string& args);

};  // namespace WChat::internal::cui
#endif
