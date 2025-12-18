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

    std::optional<uint64_t> SQLightWrapper::addUser(std::string name) {
        SQLite::Statement query(_db, "INSERT INTO users (name) VALUES (?)");
        query.bind(1, name);
        query.exec();
        logger::logger << logger::debug << "Add new user " << name << logger::endl;
        return getUserIdByName(name);
    }
    void SQLightWrapper::addContact(uint64_t userAId, uint64_t userBId) {
        SQLite::Statement query(_db, "INSERT INTO contacts (user_id_1) VALUE (?), (user_id_2) VALUE (?)");
        query.bind(1, static_cast<uint32_t>(userAId));
        query.bind(2, static_cast<uint32_t>(userBId));
        logger::logger << logger::debug << "Add Contact from " << userAId << " to " << userBId << logger::endl;
        query.exec();
    }

    std::vector<uint64_t> SQLightWrapper::getUserContacts(uint64_t userId) {
        std::vector<uint64_t> contacts;
        logger::logger << logger::debug << "Get contacts for user " << userId << logger::endl;
        SQLite::Statement query(_db, "SELECT user_id_2 FROM contacts WHERE user_id_1 = (?)");
        query.bind(1, static_cast<uint32_t>(userId));
        while (query.executeStep()) {
            uint64_t contactId = query.getColumn(0).getInt64();
            contacts.push_back(contactId);
        }
        return contacts;
    }

    std::optional<uint64_t> SQLightWrapper::getUserIdByName(const std::string& name) {
        SQLite::Statement query(_db, "SELECT id FROM users WHERE name = ?");
        query.bind(1, name);

        if (query.executeStep()) {
            int id = query.getColumn(0).getInt();
            return static_cast<uint64_t>(id);
        }

        return {};
    }

    size_t SQLightWrapper::size() {
        return 0;
    }

    bool SQLightWrapper::isUserRegistered(std::string name) {
        return false;
    }

};  // namespace WChat::ChatServer::core::storage::db::sqlite
