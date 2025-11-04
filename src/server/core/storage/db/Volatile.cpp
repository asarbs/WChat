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

    bool Volatile::isUserRegistered(std::string name) {
        auto filtered = _usersDb | std::ranges::views::filter([name](const UserInfo& ui) { return ui.name == name; });
        auto it       = filtered.begin();
        if (it != filtered.end()) {
            return true;
        }

        return false;
    }

    size_t Volatile::size() {
        return _usersDb.size();
    }

}  // namespace WChat::ChatServer::core::storage::db
