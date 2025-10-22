/*
 * World VTT
 *
 * Copyright (C) 2024, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 *
 */

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>
#include <websocketpp/server.hpp>

namespace WChat::ChatServer::client {
    class ChatClient {
        public:
            struct MsgHolder {
                    uint64_t from;
                    std::string message;
            };

            ChatClient();
            explicit ChatClient(uint64_t user_id, const std::string& name);
            ChatClient(const ChatClient& other);
            ~ChatClient();
            ChatClient& operator=(const ChatClient& other);

            bool hasMsg() const;
            bool isRegistered() const;
            const std::string& getName() const;
            MsgHolder popMsg();
            uint64_t getUserId() const;
            void registerClient();
            void saveMsg(uint64_t from, std::string message);
            void unregister();
            void addContect(std::shared_ptr<ChatClient> contact);
            std::map<uint64_t, std::shared_ptr<ChatClient>>::iterator contactsBegin();
            std::map<uint64_t, std::shared_ptr<ChatClient>>::iterator contactsEnd();
            websocketpp::connection_hdl connection;

        protected:
            //
        private:
            // Remove the ability to move
            ChatClient(ChatClient&&)            = delete;
            ChatClient& operator=(ChatClient&&) = delete;

            uint64_t _user_id;
            std::string _name;
            bool _is_registered;
            std::list<MsgHolder> _savedMsg = {};
            std::map<uint64_t, std::shared_ptr<ChatClient>> _contacts;
    };
};  // namespace WChat::ChatServer::client

#endif
