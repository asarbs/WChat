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
#include "server/client/ChatClient.h"

using namespace WChat::ChatServer::client;

class ChatClientTest : public ::testing::Test {
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};

TEST_F(ChatClientTest, CreateEmptyClient) {
    ChatClient cc;
    EXPECT_EQ(cc.getName(), "");
    EXPECT_EQ(cc.getUserId(), UINT64_MAX);
    EXPECT_FALSE(cc.isRegistered());
}

TEST_F(ChatClientTest, CreateClient) {
    ChatClient cc(14, "TestName");
    EXPECT_EQ(cc.getName(), "TestName");
    EXPECT_EQ(cc.getUserId(), 14);
    EXPECT_FALSE(cc.isRegistered());
}

TEST_F(ChatClientTest, RegisterClient) {
    ChatClient cc(14, "TestName");
    cc.registerClient();
    EXPECT_TRUE(cc.isRegistered());
    cc.unregister();
    EXPECT_FALSE(cc.isRegistered());
}

TEST_F(ChatClientTest, SaveMsg) {
    ChatClient cc(14, "TestName");
    for (uint32_t i = 10; i < 20; i++) {
        std::stringstream ss;
        ss << "Msg#" << i << std::endl;
        cc.saveMsg(i, ss.str());
    }

    while (cc.hasMsg()) {
        ChatClient::MsgHolder mh = cc.popMsg();
        EXPECT_GE(mh.from, 10);
        EXPECT_LE(mh.from, 20);
        std::stringstream ss;
        ss << "Msg#" << mh.from << std::endl;
        EXPECT_EQ(mh.message, ss.str());
    }
}
TEST_F(ChatClientTest, AddContact) {
    ChatClient cc1(14, "TestName1");
    cc1.addContect(std::make_shared<ChatClient>(15, "TestName2"));
    cc1.addContect(std::make_shared<ChatClient>(16, "TestName3"));
    for (auto iter = cc1.contactsBegin(); iter != cc1.contactsEnd(); iter++) {
        EXPECT_GE(iter->second->getUserId(), 15);
        EXPECT_LE(iter->second->getUserId(), 16);
    }
}
