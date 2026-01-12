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

#include "gtest/gtest.h"
#include "server/client/ChatClientDatabase.h"

using namespace WChat::ChatServer::client;

class ChatClientDatabaseTest : public ::testing::Test {
    protected:
        void SetUp() override {
        }

        void TearDown() override {
            ChatClientDatabase::getInstance().clean();
        }
};

TEST_F(ChatClientDatabaseTest, add_new_clinet_db) {
    websocketpp::connection_hdl hdl;
    uint32_t user_id = ChatClientDatabase::getInstance().regiserClinetSession(hdl, "asar");
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
    ASSERT_EQ(ChatClientDatabase::getInstance().isRegistered(4), 0);
}

TEST_F(ChatClientDatabaseTest, get_clinet_reference_db) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinetSession(hdl, "asar");
    ASSERT_TRUE(ChatClientDatabase::getInstance().isRegistered(0));
    ASSERT_EQ(ChatClientDatabase::getInstance().getUserIdByName("asar"), 0);
}

TEST_F(ChatClientDatabaseTest, get_unregistered_clinet) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinetSession(hdl, "asar");
    ASSERT_FALSE(ChatClientDatabase::getInstance().isRegistered(10));
}

TEST_F(ChatClientDatabaseTest, register_and_unregiseter_clinet) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinetSession(hdl, "asar");
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
    ChatClientDatabase::getInstance().unregiserClinet(0);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
}

TEST_F(ChatClientDatabaseTest, register_10_and_unregiseter_5_clinet) {
    for (uint32_t i = 0; i < 10; i++) {
        websocketpp::connection_hdl hdl;
        ChatClientDatabase::getInstance().regiserClinetSession(hdl, "asar" + std::to_string(i));
    }
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 11);
    ChatClientDatabase::getInstance().unregiserClinet(5);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 11);
    ASSERT_FALSE(ChatClientDatabase::getInstance().isRegistered(5));
    ASSERT_TRUE(ChatClientDatabase::getInstance().isRegistered(4));
    ASSERT_TRUE(ChatClientDatabase::getInstance().isRegistered(6));
    ASSERT_TRUE(ChatClientDatabase::getInstance().isRegistered(4));
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 11);
}
