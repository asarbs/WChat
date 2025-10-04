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

#include "gtest/gtest.h"
#include "serwer/ChatClient.h"
#include "serwer/ChatClientDatabase.h"

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
    ChatClientDatabase::getInstance().regiserClinet(hdl);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
}

TEST_F(ChatClientDatabaseTest, get_clinet_reference_db) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinet(hdl);
    std::optional<std::reference_wrapper<ChatClient>> opt_ref_cc = ChatClientDatabase::getInstance().get(0);
    ASSERT_TRUE(opt_ref_cc);
    std::reference_wrapper<ChatClient> ref_cc = opt_ref_cc.value();
    ChatClient& cc                            = ref_cc.get();
    ASSERT_EQ(cc.user_id, 0);
}

TEST_F(ChatClientDatabaseTest, get_unregistered_clinet) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinet(hdl);
    std::optional<std::reference_wrapper<ChatClient>> opt_ref_cc = ChatClientDatabase::getInstance().get(10);
    ASSERT_FALSE(opt_ref_cc);
}

TEST_F(ChatClientDatabaseTest, register_and_unregiseter_clinet) {
    websocketpp::connection_hdl hdl;
    ChatClientDatabase::getInstance().regiserClinet(hdl);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 1);
    ChatClientDatabase::getInstance().unregiserClinet(0);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 0);
}

TEST_F(ChatClientDatabaseTest, register_10_and_unregiseter_5_clinet) {
    for (uint32_t i = 0; i < 10; i++) {
        websocketpp::connection_hdl hdl;
        ChatClientDatabase::getInstance().regiserClinet(hdl);
    }
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 10);
    ChatClientDatabase::getInstance().unregiserClinet(5);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 9);
    std::optional<std::reference_wrapper<ChatClient>> opt_ref_cc = ChatClientDatabase::getInstance().get(5);
    ASSERT_FALSE(opt_ref_cc);

    ChatClient& cc4 = ChatClientDatabase::getInstance().get(4).value().get();
    ASSERT_EQ(cc4.user_id, 4);
    ChatClient& cc6 = ChatClientDatabase::getInstance().get(6).value().get();
    ASSERT_EQ(cc6.user_id, 6);

    ChatClient& cc4a = ChatClientDatabase::getInstance().get(4).value().get();
    ASSERT_EQ(cc4a.user_id, 4);
    ASSERT_EQ(ChatClientDatabase::getInstance().size(), 9);
}
