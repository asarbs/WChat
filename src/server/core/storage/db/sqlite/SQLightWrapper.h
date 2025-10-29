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

namespace WChat::ChatServer::core::storage::db::sqlite {
    class SQLightWrapper {
        public:
            static SQLightWrapper& instance();
            bool addUser(std::string name);

        protected:
        private:
            SQLightWrapper();
            ~SQLightWrapper();
            SQLightWrapper(const SQLightWrapper& rhs)             = delete;
            SQLightWrapper(const SQLightWrapper&& rhs)            = delete;
            SQLightWrapper& operator=(const SQLightWrapper& rhs)  = delete;
            SQLightWrapper& operator=(const SQLightWrapper&& rhs) = delete;

            SQLite::Database _db;
    };
};  // namespace WChat::ChatServer::core::storage::db::sqlite
#endif
