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

#ifndef VOLATILE_H
#define VOLATILE_H

#pragma once

#include <list>
#include <string>
#include <utility>
#include <vector>

#include "server/core/storage/Storage.h"

namespace WChat::ChatServer::core::storage::db {
    class Volatile : public Storage {
        public:
            static Volatile& instance();
            void addUser(std::string name) override;
            void addContact(uint64_t userAId, uint64_t userBId) override;
            std::vector<uint64_t> getUserContacts(uint64_t userId) override;

        protected:
        private:
            Volatile();
            ~Volatile() override;
            Volatile(const Volatile& rhs)             = delete;
            Volatile(const Volatile&& rhs)            = delete;
            Volatile& operator=(const Volatile& rhs)  = delete;
            Volatile& operator=(const Volatile&& rhs) = delete;

            struct UserInfo {
                    uint64_t userId;
                    std::string name;
                    bool isRegistered;
            };
            inline static uint64_t _userCounter = 0;
            std::list<UserInfo> _usersDb;
            std::vector<std::pair<uint64_t, uint64_t>> _contacts;
    };
};  // namespace WChat::ChatServer::core::storage::db
#endif
