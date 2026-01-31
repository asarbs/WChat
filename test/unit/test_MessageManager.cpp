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
#include "server/messages/Manager.h"
#include "server/proto/messages.pb.h"
#include "server/messages/handlers/RegisterClientSession.h"
#include "server/messages/handlers/UnregisterClientSession.h"
#include "server/messages/handlers/Message.h"
#include "server/messages/handlers/ConnectionReq.h"
#include "server/messages/handlers/ListContactReq.h"
#include "server/errors/ErrorHandlers.h"
#include <websocketpp/server.hpp>

using namespace WChat::ChatServer::messages;
using namespace WChat::ChatServer::messages::handlers;

class MessageManagerTest : public ::testing::Test {
    protected:
        void SetUp() override {
            manager = std::make_unique<Manager>();
            mock_server = std::make_unique<websocket_server>();
            
            // Initialize mock server
            mock_server->init_asio();
            mock_server->set_reuse_addr(true);
            mock_server->listen(9002);
        }

        void TearDown() override {
            // Clean up any registered handlers
        }

        std::unique_ptr<Manager> manager;
        std::unique_ptr<websocket_server> mock_server;
        websocketpp::connection_hdl mock_hdl;
};

TEST_F(MessageManagerTest, register_handler_success) {
    auto handler = std::make_shared<RegisterClientSession>();
    ASSERT_NO_THROW(manager->register_handler(WChat::MessageType::REGISTER_SESSION_REQ, handler));
}

TEST_F(MessageManagerTest, register_multiple_handlers) {
    manager->register_handler(WChat::MessageType::REGISTER_SESSION_REQ, 
                              std::make_shared<RegisterClientSession>());
    manager->register_handler(WChat::MessageType::UNREGISTER_SESSION, 
                              std::make_shared<UnregisterClientSession>());
    manager->register_handler(WChat::MessageType::SEND_TEXT_MSG, 
                              std::make_shared<Message>());
    // Should not throw
}

TEST_F(MessageManagerTest, handle_register_session_message) {
    manager->register_handler(WChat::MessageType::REGISTER_SESSION_REQ, 
                              std::make_shared<RegisterClientSession>());
    
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    auto* req = msg.mutable_registersessionreq();
    req->set_user_name("test_user");
    
    // Should throw websocketpp::exception because we can't actually send in tests
    // But the handler logic should work (registration should succeed)
    EXPECT_THROW(manager->handle(mock_server.get(), mock_hdl, msg), 
                 websocketpp::exception);
}

TEST_F(MessageManagerTest, handle_unknown_message_type) {
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::LAST);  // Invalid type
    
    // Should throw or handle gracefully
    EXPECT_ANY_THROW(manager->handle(mock_server.get(), mock_hdl, msg));
}

TEST_F(MessageManagerTest, handle_message_without_handler) {
    // Don't register any handlers
    WChat::Msg msg;
    msg.set_version(1);
    msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
    
    // Should throw since no handler is registered
    EXPECT_ANY_THROW(manager->handle(mock_server.get(), mock_hdl, msg));
}