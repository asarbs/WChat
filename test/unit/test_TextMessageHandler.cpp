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
#include "server/messages/handlers/Message.h"
#include "server/proto/messages.pb.h"
#include "server/errors/ErrorHandlers.h"
#include <websocketpp/server.hpp>

using namespace WChat::ChatServer::messages::handlers;

class TextMessageHandlerTest : public ::testing::Test {
    protected:
        void SetUp() override {
            handler = std::make_unique<Message>();
            mock_server = std::make_unique<websocket_server>();
            
            // Initialize mock server
            mock_server->init_asio();
            mock_server->set_reuse_addr(true);
            mock_server->listen(9002);
        }

        void TearDown() override {
        }

        std::unique_ptr<Message> handler;
        std::unique_ptr<websocket_server> mock_server;
        websocketpp::connection_hdl mock_hdl;
};

TEST_F(TextMessageHandlerTest, handle_valid_text_message) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
    
    auto* text_msg = msg.mutable_textmessage();
    text_msg->set_from_user_id(1);
    text_msg->set_to_user_id(2);
    text_msg->set_message("Hello World");
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}

TEST_F(TextMessageHandlerTest, handle_message_without_text_message) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
    
    // Don't set TextMessage - should throw
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 WChat::ChatServer::errors::ProtoculError);
}

TEST_F(TextMessageHandlerTest, handle_empty_message) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
    
    auto* text_msg = msg.mutable_textmessage();
    text_msg->set_from_user_id(1);
    text_msg->set_to_user_id(2);
    text_msg->set_message("");  // Empty message
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}

TEST_F(TextMessageHandlerTest, handle_long_message) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
    
    auto* text_msg = msg.mutable_textmessage();
    text_msg->set_from_user_id(1);
    text_msg->set_to_user_id(2);
    text_msg->set_message(std::string(10000, 'x'));  // Very long message
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}

TEST_F(TextMessageHandlerTest, handle_message_to_nonexistent_user) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
    
    auto* text_msg = msg.mutable_textmessage();
    text_msg->set_from_user_id(1);
    text_msg->set_to_user_id(999999);  // Non-existent user
    text_msg->set_message("Hello");
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}

TEST_F(TextMessageHandlerTest, handle_self_message) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
    
    auto* text_msg = msg.mutable_textmessage();
    text_msg->set_from_user_id(1);
    text_msg->set_to_user_id(1);  // Self message
    text_msg->set_message("Hello self");
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}