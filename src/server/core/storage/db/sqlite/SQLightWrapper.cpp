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

#include "SQLightWrapper.h"

#include "logger.h"

namespace WChat::ChatServer::core::storage::db::sqlite {
    SQLightWrapper::SQLightWrapper() : _db("WCHat.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
        try {
            _db.exec("PRAGMA foreign_keys = ON;");
            _db.exec("CREATE TABLE IF NOT EXISTS users    (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, created_at DATETIME DEFAULT CURRENT_TIMESTAMP);");
            _db.exec(
                "CREATE TABLE IF NOT EXISTS contacts ("
                "user_id_1 INTEGER NOT NULL, "
                "user_id_2 INTEGER NOT NULL, "
                "FOREIGN KEY (user_id_1) REFERENCES users(id) ON DELETE CASCADE, "
                "FOREIGN KEY (user_id_2) REFERENCES users(id) ON DELETE CASCADE, "
                "UNIQUE (user_id_1, user_id_2)"
                ");");

            logger::logger << logger::info << "Add tables" << logger::endl;
        } catch (const std::exception& e) {
            logger::logger << logger::warning << "Błąd SQLite: " << e.what() << logger::endl;
        }
    }

    SQLightWrapper::~SQLightWrapper() {
    }
    SQLightWrapper& SQLightWrapper::instance() {
        static SQLightWrapper instance;
        return instance;
    }

    bool SQLightWrapper::addUser(std::string name) {  // cppcheck-suppress unusedFunction
        SQLite::Statement query(_db, "INSERT INTO users (name) VALUES (?)");
        query.bind(1, name);
        query.exec();
        return true;
    }

};  // namespace WChat::ChatServer::core::storage::db::sqlite
