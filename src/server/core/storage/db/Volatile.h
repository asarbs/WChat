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

            bool createConnection(uint64_t from, uint64_t to) override;
            bool hasMsg(uint64_t userId) override;
            bool isUserRegistered(const std::string& name, std::string password_hash) override;
            bool isUserRegistered(uint64_t userId) override;
            bool registerUser(uint64_t userId) override;
            bool saveMsg(uint64_t to, uint64_t from, const std::string& message, bool wasSend) override;
            bool unregister(uint64_t userId) override;
            size_t size() override;
            std::optional<uint64_t> addUser(std::string name, std::string password_hash) override;
            std::optional<uint64_t> getUserIdByName(const std::string& name, const std::string& password_hash) override;
            std::vector<uint64_t> getUserContacts(uint64_t userId) override;
            void addContact(uint64_t userAId, uint64_t userBId) override;
            MsgHolder popMsg(uint64_t user_id) override;
            Contacts getContacts(uint64_t userId) override;
            void clean() override;

        protected:
            //
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
            struct MessageInfo {
                    uint64_t to;
                    uint64_t from;
                    std::string message;
                    bool wasSent;
            };
            inline static uint64_t _userCounter = 0;
            std::list<UserInfo> _usersDb;
            std::vector<std::pair<uint64_t, uint64_t>> _contacts;
            std::list<MessageInfo> _messages;
    };
};  // namespace WChat::ChatServer::core::storage::db
#endif
