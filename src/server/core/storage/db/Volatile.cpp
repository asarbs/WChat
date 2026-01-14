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

    std::optional<uint64_t> Volatile::addUser(std::string name) {
        _usersDb.emplace_back(_userCounter, name, true);
        _userCounter++;

        return _userCounter - 1;
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

    std::optional<uint64_t> Volatile::getUserIdByName(const std::string& name) {
        auto filtered = _usersDb | std::ranges::views::filter([name](const UserInfo& ui) { return ui.name == name; });
        auto it       = filtered.begin();
        if (it != filtered.end()) {
            return it->userId;
        }

        return {};
    }

    bool Volatile::isUserRegistered(const std::string& name) {
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
        return false;
    }

    bool Volatile::saveMsg(uint64_t to, uint64_t from, const std::string& message, bool wasSend) {
        return false;
    }

    bool Volatile::createConnection(uint64_t from, uint64_t to) {
        return false;
    }

    MsgHolder Volatile::popMsg(uint64_t user_id) {
        return MsgHolder{0, "TEST MSG"};
    }

    Contacts Volatile::getContacts(uint64_t userId) {
        return {};
    }

    void Volatile::clean() {
        _userCounter = 0;
        _usersDb.clear();
        _contacts.clear();
        logger::logger << logger::debug << "Volatile::clean()" << logger::endl;
    }

}  // namespace WChat::ChatServer::core::storage::db
