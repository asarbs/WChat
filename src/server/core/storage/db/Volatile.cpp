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

    void Volatile::addUser(std::string name) {
        _usersDb.emplace_back(_userCounter, name, true);
        _userCounter++;
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

}  // namespace WChat::ChatServer::core::storage::db
