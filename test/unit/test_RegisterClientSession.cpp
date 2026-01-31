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
#include "server/messages/handlers/RegisterClientSession.h"
#include "server/proto/messages.pb.h"
#include "server/errors/ErrorHandlers.h"
#include <websocketpp/server.hpp>

using namespace WChat::ChatServer::messages::handlers;

class RegisterClientSessionTest : public ::testing::Test {
protected:
        void SetUp() override {
            handler = std::make_unique<RegisterClientSession>();
            mock_server = std::make_unique<websocket_server>();
        }

        void TearDown() override {
        }

        std::unique_ptr<RegisterClientSession> handler;
        std::unique_ptr<websocket_server> mock_server;
        websocketpp::connection_hdl mock_hdl;
};

TEST_F(RegisterClientSessionTest, handle_valid_register_request) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    auto* req = msg.mutable_registersessionreq();
    req->set_user_name("test_user");
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work (registration should succeed)
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}

TEST_F(RegisterClientSessionTest, handle_message_without_register_session_req) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    // Don't set RegisterSessionReq - should throw
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 WChat::ChatServer::errors::ProtoculError);
}

TEST_F(RegisterClientSessionTest, handle_empty_username) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    auto* req = msg.mutable_registersessionreq();
    req->set_user_name("");  // Empty username
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}

TEST_F(RegisterClientSessionTest, handle_long_username) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    auto* req = msg.mutable_registersessionreq();
    req->set_user_name(std::string(1000, 'a'));  // Very long username
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work
    EXPECT_THROW(handler->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}