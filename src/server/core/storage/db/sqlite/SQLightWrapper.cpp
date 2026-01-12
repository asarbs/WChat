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

#include <utility>

#include "logger.h"

namespace WChat::ChatServer::core::storage::db::sqlite {
    SQLightWrapper::SQLightWrapper() : _db("WCHat.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
        try {
            _db.exec("PRAGMA foreign_keys = ON;");
            _db.exec(
                "CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name TEXT NOT NULL, "
                "isRegistered INTEGER NOT NULL DEFAULT 0, "
                "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                ");");
            _db.exec(
                "CREATE TABLE IF NOT EXISTS contacts ("
                "user_id_1 INTEGER NOT NULL, "
                "user_id_2 INTEGER NOT NULL, "
                "FOREIGN KEY (user_id_1) REFERENCES users(id) ON DELETE CASCADE, "
                "FOREIGN KEY (user_id_2) REFERENCES users(id) ON DELETE CASCADE, "
                "UNIQUE (user_id_1, user_id_2)"
                ");");
            _db.exec(
                "CREATE TABLE IF NOT EXISTS messages ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "user_from INTEGER NOT NULL, "
                "user_to INTEGER NOT NULL, "
                "msg_text TEXT NOT NULL, "
                "was_sent INTEGER NOT NULL DEFAULT 0, "
                "was_delivered INTEGER NOT NULL DEFAULT 0, "
                "FOREIGN KEY (user_from) REFERENCES users(id) ON DELETE CASCADE, "
                "FOREIGN KEY (user_to) REFERENCES users(id) ON DELETE CASCADE "
                ");");
            _db.exec(
                "CREATE INDEX IF NOT EXISTS idx_messages_user_to "
                "ON messages(user_to);");

            _db.exec(
                "CREATE INDEX IF NOT EXISTS idx_messages_user_from "
                "ON messages(user_from);");

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

    void SQLightWrapper::clean() {
        SQLite::Transaction transaction(_db);

        _db.exec("DELETE FROM messages;");
        _db.exec("DELETE FROM contacts;");
        _db.exec("DELETE FROM users;");

        transaction.commit();
    }

    std::optional<uint64_t> SQLightWrapper::addUser(std::string name) {
        SQLite::Statement query(_db, "INSERT INTO users (name, isRegistered) VALUES (?, ?)");
        query.bind(1, name);
        query.bind(2, true);
        query.exec();
        logger::logger << logger::debug << "Add new user " << name << logger::endl;
        return getUserIdByName(name);
    }

    bool SQLightWrapper::unregister(uint64_t userId) {
        SQLite::Statement query(_db, "UPDATE users isRegistered = ? WHERE id = ?");

        query.bind(1, false);
        query.bind(2, uint32_t(userId));

        query.exec();
        return true;
    }

    bool SQLightWrapper::registerUser(uint64_t userId) {
        SQLite::Statement query(_db, "UPDATE users isRegistered = ? WHERE id = ?");

        query.bind(1, false);
        query.bind(2, uint32_t(userId));

        query.exec();
        return true;
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
        SQLite::Statement query(_db, "SELECT COUNT(*) FROM users");
        if (query.executeStep()) {
            size_t count = query.getColumn(0).getUInt();
            return count;
        }
        return 0;
    }

    bool SQLightWrapper::isUserRegistered(const std::string& name) {
        SQLite::Statement query(_db, "SELECT isRegistered FROM users WHERE name = (?)");
        query.bind(1, name);
        if (query.executeStep()) {
            uint32_t isRegistered = query.getColumn(0).getUInt();
            return isRegistered == 1;
        }
        return false;
    }

    bool SQLightWrapper::isUserRegistered(uint64_t userId) {
        SQLite::Statement query(_db, "SELECT isRegistered FROM users WHERE id = (?)");
        query.bind(1, static_cast<uint32_t>(userId));
        if (query.executeStep()) {
            uint32_t isRegistered = query.getColumn(0).getUInt();
            return isRegistered == 1;
        }
        return false;
    }

    bool SQLightWrapper::hasMsg(uint64_t userId) {
        SQLite::Statement query(_db, "SELECT COUNT(*) FROM messages WHERE user_to = ?");
        query.bind(1, static_cast<uint32_t>(userId));
        if (query.executeStep()) {
            size_t count = query.getColumn(0).getUInt();
            return count > 0;
        }
        return 0;
    }

    bool SQLightWrapper::saveMsg(uint64_t to, uint64_t from, const std::string& message, bool wasSend) {
        SQLite::Statement query(_db, "INSERT INTO messages (user_from, user_to, , msg_text, was_sent) VALUE (?, ?, ?, ?)");
        query.bind(1, static_cast<uint32_t>(from));
        query.bind(2, static_cast<uint32_t>(to));
        query.bind(3, message);
        query.bind(4, static_cast<uint32_t>(wasSend));
        logger::logger << logger::debug << "Add Msg [" << from << ", " << to << ", " << message << "] saved" << logger::endl;
        query.exec();
        return true;
    }

    bool SQLightWrapper::createConnection(uint64_t from, uint64_t to) {
        SQLite::Statement query(_db, "INSERT INTO contacts (user_id_1, user_id_2) VALUE (?, ?)");
        query.bind(1, static_cast<uint32_t>(from));
        query.bind(2, static_cast<uint32_t>(to));
        logger::logger << logger::debug << "Add Contacts fron " << from << " to " << to << " saved" << logger::endl;
        query.exec();
        return true;
    }

    MsgHolder SQLightWrapper::popMsg(uint64_t user_id) {
        SQLite::Statement query(_db,
                                "SELECT id, user_from, msg_text "
                                "FROM messages "
                                "WHERE user_to = ? AND was_delivered = 0 "
                                "ORDER BY id ASC "
                                "LIMIT 1");
        query.bind(1, static_cast<uint32_t>(user_id));

        if (!query.executeStep()) {
            throw std::runtime_error("No messages for user");
        }

        const uint32_t msgId = query.getColumn(0).getUInt();
        MsgHolder msg{query.getColumn(1).getUInt(), query.getColumn(2).getString()};

        // oznacz jako odebraną
        SQLite::Statement update(_db, "UPDATE messages SET was_sent = 1 WHERE id = ?");
        update.bind(1, msgId);
        update.exec();

        return msg;
    }

    Contacts SQLightWrapper::getContacts(uint64_t userId) {
        Contacts result;

        SQLite::Statement query(_db,
                                "SELECT u.id, u.name "
                                "FROM contacts c "
                                "JOIN users u ON u.id = "
                                "   CASE "
                                "       WHEN c.user_id_1 = ? THEN c.user_id_2 "
                                "       ELSE c.user_id_1 "
                                "   END "
                                "WHERE c.user_id_1 = ? OR c.user_id_2 = ?;");

        query.bind(1, static_cast<uint32_t>(userId));
        query.bind(2, static_cast<uint32_t>(userId));
        query.bind(3, static_cast<uint32_t>(userId));

        while (query.executeStep()) {
            ContactInfo info;
            info.contact_id = static_cast<uint32_t>(query.getColumn(0).getInt64());
            info.name       = query.getColumn(1).getString();

            result.push_back(std::move(info));
        }

        return result;
    }

};  // namespace WChat::ChatServer::core::storage::db::sqlite
