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

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace WChat::ChatClient {
    class ChatClient {
        public:
            ChatClient();
            virtual ~ChatClient();
            ChatClient(const ChatClient& rhs);
            ChatClient(const ChatClient&& rhs);
            ChatClient& operator=(const ChatClient& rhs);
            ChatClient& operator=(const ChatClient&& rhs);

            void setUserId(uint64_t userId);
            bool isRegistered() const;
            uint64_t getUserId() const;
            void setName(const std::string& name);
            const std::string& getName() const;
            bool hasName() const;

            void addContact(uint64_t contactId, const std::string& contactName) {
                _contacts.insert({contactName, contactId});
            }

            uint64_t getContactId(const std::string& contactName) {
                return _contacts.at(contactName);
            }

            const std::unordered_map<std::string, uint64_t>& getContacts() const;

        protected:
            //
        private:
            uint64_t _userId;
            std::string _name;

            std::unordered_map<std::string, uint64_t> _contacts;
    };
};  // namespace WChat::ChatClient
#endif
