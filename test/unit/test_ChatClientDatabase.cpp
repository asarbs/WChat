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
#include "server/client/ChatClient.h"
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
    uint32_t user_id = ChatClientDatabase::getInstance().regiserClinet(hdl, "asar");
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
    ASSERT_EQ(user_id, 0);
}

TEST_F(ChatClientDatabaseTest, get_clinet_reference_db) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinet(hdl, "asar");
    std::shared_ptr<ChatClient> cc = ChatClientDatabase::getInstance().get(0);
    ASSERT_TRUE(cc);
    ASSERT_EQ(cc->getUserId(), 0);
}

TEST_F(ChatClientDatabaseTest, get_unregistered_clinet) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinet(hdl, "asar");
    std::shared_ptr<ChatClient> cc = ChatClientDatabase::getInstance().get(10);
    ASSERT_FALSE(cc);
}

TEST_F(ChatClientDatabaseTest, register_and_unregiseter_clinet) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinet(hdl, "asar");
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
    ChatClientDatabase::getInstance().unregiserClinet(0);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
}

TEST_F(ChatClientDatabaseTest, register_10_and_unregiseter_5_clinet) {
    for (uint32_t i = 0; i < 10; i++) {
        websocketpp::connection_hdl hdl;
        ChatClientDatabase::getInstance().regiserClinet(hdl, "asar" + std::to_string(i));
    }
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 10);
    ChatClientDatabase::getInstance().unregiserClinet(5);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 10);
    std::shared_ptr<ChatClient> cc = ChatClientDatabase::getInstance().get(5);
    ASSERT_TRUE(cc);
    ASSERT_FALSE(cc->isRegistered());

    std::shared_ptr<ChatClient> cc4 = ChatClientDatabase::getInstance().get(4);
    ASSERT_EQ(cc4->getUserId(), 4);
    ASSERT_TRUE(cc4->isRegistered());
    std::shared_ptr<ChatClient> cc6 = ChatClientDatabase::getInstance().get(6);
    ASSERT_EQ(cc6->getUserId(), 6);
    ASSERT_TRUE(cc6->isRegistered());

    std::shared_ptr<ChatClient> cc4a = ChatClientDatabase::getInstance().get(4);
    ASSERT_EQ(cc4a->getUserId(), 4);
    ASSERT_TRUE(cc4->isRegistered());
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 10);
}
