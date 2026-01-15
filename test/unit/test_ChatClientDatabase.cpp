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
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 10);
    ASSERT_TRUE(ChatClientDatabase::getInstance().unregiserClinet(5));
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 10);
    ASSERT_FALSE(ChatClientDatabase::getInstance().isRegistered(5));
    ASSERT_TRUE(ChatClientDatabase::getInstance().isRegistered(4));
    ASSERT_TRUE(ChatClientDatabase::getInstance().isRegistered(6));
    ASSERT_TRUE(ChatClientDatabase::getInstance().isRegistered(4));
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 10);
}

TEST_F(ChatClientDatabaseTest, create_connection_between_clients) {
    websocketpp::connection_hdl hdl1, hdl2;
    uint32_t user1 = ChatClientDatabase::getInstance().regiserClinetSession(hdl1, "user1");
    uint32_t user2 = ChatClientDatabase::getInstance().regiserClinetSession(hdl2, "user2");
    
    ASSERT_TRUE(ChatClientDatabase::getInstance().createConnection(user1, user2));
}

TEST_F(ChatClientDatabaseTest, create_connection_with_nonexistent_client) {
    ASSERT_FALSE(ChatClientDatabase::getInstance().createConnection(999, 1000));
}

TEST_F(ChatClientDatabaseTest, save_and_check_messages) {
    websocketpp::connection_hdl hdl1, hdl2;
    uint32_t user1 = ChatClientDatabase::getInstance().regiserClinetSession(hdl1, "sender");
    uint32_t user2 = ChatClientDatabase::getInstance().regiserClinetSession(hdl2, "receiver");
    
    ASSERT_TRUE(ChatClientDatabase::getInstance().saveMsg(user2, user1, "Hello", false));
    ASSERT_TRUE(ChatClientDatabase::getInstance().hasMsg(user2));
    ASSERT_FALSE(ChatClientDatabase::getInstance().hasMsg(user1));
}

TEST_F(ChatClientDatabaseTest, pop_message_from_queue) {
    websocketpp::connection_hdl hdl1, hdl2;
    uint32_t user1 = ChatClientDatabase::getInstance().regiserClinetSession(hdl1, "sender");
    uint32_t user2 = ChatClientDatabase::getInstance().regiserClinetSession(hdl2, "receiver");
    
    ChatClientDatabase::getInstance().saveMsg(user2, user1, "Test message", false);
    ASSERT_TRUE(ChatClientDatabase::getInstance().hasMsg(user2));
    
    auto msgHolder = ChatClientDatabase::getInstance().popMsg(user2);
    ASSERT_EQ(msgHolder.from, user1);
    ASSERT_EQ(msgHolder.message, "Test message");
}

TEST_F(ChatClientDatabaseTest, get_client_contacts) {
    websocketpp::connection_hdl hdl1, hdl2;
    uint32_t user1 = ChatClientDatabase::getInstance().regiserClinetSession(hdl1, "user1");
    uint32_t user2 = ChatClientDatabase::getInstance().regiserClinetSession(hdl2, "user2");
    
    ChatClientDatabase::getInstance().createConnection(user1, user2);
    auto contacts = ChatClientDatabase::getInstance().getContacts(user1);
    ASSERT_GT(contacts.size(), 0);
}

TEST_F(ChatClientDatabaseTest, get_websocket_connection) {
    // Test that connection method returns something (even if empty)
    // In real scenario this would return the stored connection
    uint32_t nonexistent_user = 999;
    auto connection = ChatClientDatabase::getInstance().connection(nonexistent_user);
    auto retrievedHdl = connection.lock();
    ASSERT_TRUE(retrievedHdl == nullptr);  // Should be null for nonexistent user
}

TEST_F(ChatClientDatabaseTest, get_connection_for_nonexistent_user) {
    auto connection = ChatClientDatabase::getInstance().connection(999);
    ASSERT_TRUE(connection.lock() == nullptr);
}

TEST_F(ChatClientDatabaseTest, register_client_session_by_id) {
    uint32_t user_id = 12345;
    uint32_t result = ChatClientDatabase::getInstance().regiserClinetSession(user_id);
    ASSERT_EQ(result, user_id);
}
