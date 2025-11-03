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
    class Storage {
        public:
            virtual ~Storage() = default;

            virtual bool isUserRegistered(std::string name)                          = 0;
            virtual size_t size()                                                    = 0;
            virtual std::vector<uint64_t> getUserContacts(uint64_t userId)           = 0;
            virtual std::optional<uint64_t> getUserIdByName(const std::string& name) = 0;
            virtual void addContact(uint64_t userAId, uint64_t userBId)              = 0;
            virtual void addUser(std::string name)                                   = 0;

        protected:
            Storage() = default;

        private:
    };
};  // namespace WChat::ChatServer::core::storage
#endif
