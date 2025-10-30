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
#ifndef SQLIGHTWRAPPER_H
#define SQLIGHTWRAPPER_H

#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include <string>
#include <vector>

#include "server/core/storage/Storage.h"

namespace WChat::ChatServer::core::storage::db::sqlite {
    class SQLightWrapper : public Storage {
        public:
            static SQLightWrapper& instance();
            void addUser(std::string name) override;
            void addContact(uint64_t userAId, uint64_t userBId) override;
            std::vector<uint64_t> getUserContacts(uint64_t userId) override;

        protected:
        private:
            SQLightWrapper();
            ~SQLightWrapper() override;
            SQLightWrapper(const SQLightWrapper& rhs)             = delete;
            SQLightWrapper(const SQLightWrapper&& rhs)            = delete;
            SQLightWrapper& operator=(const SQLightWrapper& rhs)  = delete;
            SQLightWrapper& operator=(const SQLightWrapper&& rhs) = delete;

            SQLite::Database _db;
    };
};  // namespace WChat::ChatServer::core::storage::db::sqlite
#endif
