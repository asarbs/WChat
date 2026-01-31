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
#include "server/proto/messages.pb.h"

class ProtocolBuffersTest : public ::testing::Test {
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};

TEST_F(ProtocolBuffersTest, create_register_session_request) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    auto* req = msg.mutable_registersessionreq();
    req->set_user_name("testuser");
    
    EXPECT_EQ(msg.version(), 1);
    EXPECT_EQ(msg.type(), WChat::MessageType::REGISTER_SESSION_REQ);
    EXPECT_TRUE(msg.has_registersessionreq());
    EXPECT_EQ(msg.registersessionreq().user_name(), "testuser");
}

TEST_F(ProtocolBuffersTest, create_text_message) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
    
    auto* text_msg = msg.mutable_textmessage();
    text_msg->set_from_user_id(1);
    text_msg->set_to_user_id(2);
    text_msg->set_message("Hello World");
    
    EXPECT_EQ(msg.version(), 1);
    EXPECT_EQ(msg.type(), WChat::MessageType::SEND_TEXT_MSG);
    EXPECT_TRUE(msg.has_textmessage());
    EXPECT_EQ(msg.textmessage().from_user_id(), 1);
    EXPECT_EQ(msg.textmessage().to_user_id(), 2);
    EXPECT_EQ(msg.textmessage().message(), "Hello World");
}

TEST_F(ProtocolBuffersTest, create_connection_request) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::CONTACT_CONNECTION_REQ);
    
    auto* conn_req = msg.mutable_contactconnectionreq();
    conn_req->set_from_user_id(1);
    conn_req->set_to_user_id(2);
    conn_req->set_from_user_name("user1");
    
    EXPECT_EQ(msg.type(), WChat::MessageType::CONTACT_CONNECTION_REQ);
    EXPECT_TRUE(msg.has_contactconnectionreq());
    EXPECT_EQ(msg.contactconnectionreq().from_user_id(), 1);
    EXPECT_EQ(msg.contactconnectionreq().to_user_id(), 2);
    EXPECT_EQ(msg.contactconnectionreq().from_user_name(), "user1");
}

TEST_F(ProtocolBuffersTest, create_register_session_response) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_RES);
    
    auto* res = msg.mutable_registersessionres();
    res->set_status(WChat::Response::ACK);
    res->set_user_id(123);
    
    EXPECT_EQ(msg.type(), WChat::MessageType::REGISTER_SESSION_RES);
    EXPECT_TRUE(msg.has_registersessionres());
    EXPECT_EQ(msg.registersessionres().status(), WChat::Response::ACK);
    EXPECT_EQ(msg.registersessionres().user_id(), 123);
}

TEST_F(ProtocolBuffersTest, serialize_and_deserialize) {
    // Create original message
    WChat::Msg original_msg;
    original_msg.set_version(1);
    original_msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    auto* req = original_msg.mutable_registersessionreq();
    req->set_user_name("serialized_user");
    
    // Serialize to string
    std::string serialized = original_msg.SerializeAsString();
    EXPECT_FALSE(serialized.empty());
    
    // Deserialize from string
    WChat::Msg deserialized_msg;
    EXPECT_TRUE(deserialized_msg.ParseFromString(serialized));
    
    // Verify deserialized message matches original
    EXPECT_EQ(deserialized_msg.version(), original_msg.version());
    EXPECT_EQ(deserialized_msg.type(), original_msg.type());
    EXPECT_EQ(deserialized_msg.registersessionreq().user_name(), 
              original_msg.registersessionreq().user_name());
}

TEST_F(ProtocolBuffersTest, message_without_optional_fields) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    // Don't set optional RegisterSessionReq field
    
    EXPECT_EQ(msg.version(), 1);
    EXPECT_EQ(msg.type(), WChat::MessageType::REGISTER_SESSION_REQ);
    EXPECT_FALSE(msg.has_registersessionreq());
}

TEST_F(ProtocolBuffersTest, invalid_deserialization) {
    std::string invalid_data = "This is not valid protobuf data";
    
    WChat::Msg msg;
    EXPECT_FALSE(msg.ParseFromString(invalid_data));
}

TEST_F(ProtocolBuffersTest, create_list_contact_request) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::LIST_CONTACT_REQ);
    
    auto* list_req = msg.mutable_listcontactreq();
    list_req->set_user_id(42);
    
    EXPECT_EQ(msg.type(), WChat::MessageType::LIST_CONTACT_REQ);
    EXPECT_TRUE(msg.has_listcontactreq());
    EXPECT_EQ(msg.listcontactreq().user_id(), 42);
}

TEST_F(ProtocolBuffersTest, create_list_contact_response) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::LIST_CONTACT_RES);
    
    auto* list_res = msg.mutable_listcontactres();
    
    // Add contact
    auto* contact = list_res->add_contacts();
    contact->set_user_name("contact1");
    contact->set_user_id(1);
    
    EXPECT_EQ(msg.type(), WChat::MessageType::LIST_CONTACT_RES);
    EXPECT_TRUE(msg.has_listcontactres());
    EXPECT_EQ(list_res->contacts_size(), 1);
    EXPECT_EQ(list_res->contacts(0).user_name(), "contact1");
    EXPECT_EQ(list_res->contacts(0).user_id(), 1);
}

TEST_F(ProtocolBuffersTest, response_values) {
    EXPECT_EQ(static_cast<int>(WChat::Response::NACK), 0);
    EXPECT_EQ(static_cast<int>(WChat::Response::ACK), 1);
}

TEST_F(ProtocolBuffersTest, message_type_values) {
    EXPECT_EQ(static_cast<int>(WChat::MessageType::RESPONSE), 0);
    EXPECT_EQ(static_cast<int>(WChat::MessageType::REGISTER_SESSION_REQ), 1);
    EXPECT_EQ(static_cast<int>(WChat::MessageType::REGISTER_SESSION_RES), 2);
    EXPECT_EQ(static_cast<int>(WChat::MessageType::SEND_TEXT_MSG), 3);
}