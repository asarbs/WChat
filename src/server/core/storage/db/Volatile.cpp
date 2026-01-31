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

#include "Volatile.h"

#include <algorithm>
#include <ranges>
#include <utility>
#include <vector>

#include "logger.h"

namespace WChat::ChatServer::core::storage::db {
    Volatile::Volatile() {
    }

    Volatile::~Volatile() {
    }

    Volatile& Volatile::instance() {
        static Volatile instance;
        return instance;
    }

    std::optional<uint64_t> Volatile::addUser(std::string name, std::string password_hash) {
        // First, check if user already exists
        auto filtered = _usersDb | std::ranges::views::filter([&name](const UserInfo& ui) { return ui.name == name; });
        auto it       = filtered.begin();

        if (it != filtered.end()) {
            // User exists - return existing user ID (password hash is ignored for this test)
            return it->userId;
        }

        // User doesn't exist - create new user
        uint64_t new_user_id = _userCounter;
        _usersDb.emplace_back(new_user_id, name, true);
        _userCounter++;
        return new_user_id;
    }
    bool Volatile::unregister(uint64_t userId) {
        auto filtered = _usersDb | std::ranges::views::filter([userId](const UserInfo& ui) { return ui.userId == userId; });
        auto it       = filtered.begin();
        if (it != filtered.end()) {
            logger::logger << logger::debug << "registerUser userId = " << userId << ", it->userId=" << it->userId << " Success" << logger::endl;
            it->isRegistered = false;
            return true;
        }
        logger::logger << logger::warning << "registerUser userId = " << userId << ", it->userId=" << it->userId << " Failed" << logger::endl;
        return false;
    }

    bool Volatile::registerUser(uint64_t userId) {
        auto filtered = _usersDb | std::ranges::views::filter([userId](const UserInfo& ui) { return ui.userId == userId; });
        auto it       = filtered.begin();
        if (it != filtered.end()) {
            logger::logger << logger::debug << "registerUser userId = " << userId << ", it->userId=" << it->userId << " Success" << logger::endl;
            it->isRegistered = true;
            return true;
        }
        logger::logger << logger::warning << "registerUser userId = " << userId << ", it->userId=" << it->userId << " Failed" << logger::endl;
        return false;
    }
    void Volatile::addContact(uint64_t userAId, uint64_t userBId) {
        _contacts.emplace_back(userAId, userBId);
    }

    std::vector<uint64_t> Volatile::getUserContacts(uint64_t userId) {
        auto filtered = _contacts                                                                                                     //
                        | std::ranges::views::filter([userId](const std::pair<uint64_t, uint64_t>& p) { return p.first == userId; })  //
                        | std::ranges::views::transform([](const std::pair<uint64_t, uint64_t>& p) { return p.second; });
        return std::vector<uint64_t>(filtered.begin(), filtered.end());
    }

    std::optional<uint64_t> Volatile::getUserIdByName(const std::string& name, const std::string& password_hash) {
        auto filtered = _usersDb | std::ranges::views::filter([name](const UserInfo& ui) { return ui.name == name; });
        auto it       = filtered.begin();
        if (it != filtered.end()) {
            return it->userId;
        }

        return {};
    }

    bool Volatile::isUserRegistered(const std::string& name, std::string password_hash) {
        auto filtered = _usersDb | std::ranges::views::filter([name](const UserInfo& ui) { return ui.name == name; });
        auto it       = filtered.begin();
        logger::logger << logger::debug << "User name = " << name << " is registered" << logger::endl;
        if (it != filtered.end()) {
            return true;
        }

        return false;
    }

    bool Volatile::isUserRegistered(uint64_t userId) {
        auto filtered = _usersDb | std::ranges::views::filter([userId](const UserInfo& ui) { return ui.userId == userId; });
        auto it       = filtered.begin();
        if (it != filtered.end()) {
            logger::logger << logger::debug << "User id = " << userId << " is registered" << logger::endl;
            return it->isRegistered;
        }

        return false;
    }

    size_t Volatile::size() {
        return _usersDb.size();
    }

    bool Volatile::hasMsg(uint64_t userId) {
        auto filtered = _messages | std::ranges::views::filter([userId](const MessageInfo& msg) { return msg.to == userId; });
        return !filtered.empty();
    }

    bool Volatile::saveMsg(uint64_t to, uint64_t from, const std::string& message, bool wasSend) {
        _messages.emplace_back(to, from, message, wasSend);
        return true;
    }

    bool Volatile::createConnection(uint64_t from, uint64_t to) {
        auto fromUser = std::find_if(_usersDb.begin(), _usersDb.end(), [from](const UserInfo& ui) { return ui.userId == from && ui.isRegistered; });
        auto toUser   = std::find_if(_usersDb.begin(), _usersDb.end(), [to](const UserInfo& ui) { return ui.userId == to && ui.isRegistered; });

        if (fromUser != _usersDb.end() && toUser != _usersDb.end()) {
            _contacts.emplace_back(from, to);
            return true;
        }
        return false;
    }

    MsgHolder Volatile::popMsg(uint64_t user_id) {
        auto it = std::find_if(_messages.begin(), _messages.end(), [user_id](const MessageInfo& msg) { return msg.to == user_id; });
        if (it != _messages.end()) {
            MsgHolder result{it->from, it->message};
            _messages.erase(it);
            return result;
        }
        return MsgHolder{0, ""};
    }

    Contacts Volatile::getContacts(uint64_t userId) {
        Contacts result;
        auto contactIds = getUserContacts(userId);

        for (auto contactId : contactIds) {
            auto userIt = std::find_if(_usersDb.begin(), _usersDb.end(), [contactId](const UserInfo& ui) { return ui.userId == contactId; });
            if (userIt != _usersDb.end()) {
                result.emplace_back(userIt->name, static_cast<uint32_t>(userIt->userId));
            }
        }
        return result;
    }

    void Volatile::clean() {
        _userCounter = 0;
        _usersDb.clear();
        _contacts.clear();
        _messages.clear();
        logger::logger << logger::debug << "Volatile::clean()" << logger::endl;
    }

}  // namespace WChat::ChatServer::core::storage::db
