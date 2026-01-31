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

#ifndef STORAGE_H
#define STORAGE_H

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace WChat::ChatServer::core::storage {

    struct MsgHolder {
            uint64_t from;
            std::string message;
    };

    struct ContactInfo {
            std::string name;
            uint32_t contact_id;
    };
    using Contacts = std::vector<ContactInfo>;

    class Storage {
        public:
            virtual ~Storage() = default;

            virtual bool isUserRegistered(const std::string& name, std::string password_hash)                          = 0;
            virtual bool isUserRegistered(uint64_t userId)                                                             = 0;
            virtual size_t size()                                                                                      = 0;
            virtual std::vector<uint64_t> getUserContacts(uint64_t userId)                                             = 0;
            virtual std::optional<uint64_t> getUserIdByName(const std::string& name, const std::string& password_hash) = 0;
            virtual void addContact(uint64_t userAId, uint64_t userBId)                                                = 0;
            virtual std::optional<uint64_t> addUser(std::string name, std::string password_hash)                       = 0;
            virtual bool unregister(uint64_t userId)                                                                   = 0;
            virtual bool registerUser(uint64_t userId)                                                                 = 0;
            virtual bool hasMsg(uint64_t userId)                                                                       = 0;
            virtual bool saveMsg(uint64_t to, uint64_t from, const std::string& message, bool wasSend)                 = 0;
            virtual bool createConnection(uint64_t from, uint64_t to)                                                  = 0;
            virtual MsgHolder popMsg(uint64_t user_id)                                                                 = 0;
            virtual Contacts getContacts(uint64_t userId)                                                              = 0;
            virtual void clean()                                                                                       = 0;

        protected:
            Storage() = default;

        private:
    };
};  // namespace WChat::ChatServer::core::storage
#endif
