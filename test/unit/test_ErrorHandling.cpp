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
#include "server/errors/ClientException.h"
#include "server/errors/ErrorHandlers.h"

using namespace WChat::ChatServer::errors;
using namespace WChat::ChatServer::errors::client;

class ErrorHandlingTest : public ::testing::Test {
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};

TEST_F(ErrorHandlingTest, protocol_error_creation) {
    std::string error_msg = "Test protocol error";
    ProtoculError error(error_msg);
    
    EXPECT_STREQ(error.what(), error_msg.c_str());
}

TEST_F(ErrorHandlingTest, protocol_warning_creation) {
    std::string warning_msg = "Test protocol warning";
    ProtoculWarning warning(warning_msg);
    
    EXPECT_STREQ(warning.what(), warning_msg.c_str());
}

TEST_F(ErrorHandlingTest, client_exception_creation) {
    std::string exception_msg = "Test client exception";
    ClientException exception(exception_msg);
    
    EXPECT_STREQ(exception.what(), exception_msg.c_str());
}

TEST_F(ErrorHandlingTest, exceptions_are_inheritable) {
    // Test that our custom exceptions can be caught as std::exception
    try {
        throw ProtoculError("Test error");
    } catch (const std::exception& e) {
        EXPECT_STREQ(e.what(), "Test error");
    } catch (...) {
        ADD_FAILURE() << "Exception should be catchable as std::exception";
    }
}

TEST_F(ErrorHandlingTest, different_exception_types) {
    // Test that different exception types can be differentiated
    bool caught_error = false;
    bool caught_warning = false;
    
    try {
        throw ProtoculError("Error");
    } catch (const ProtoculError& e) {
        caught_error = true;
    } catch (...) {
        ADD_FAILURE() << "Should catch ProtoculError";
    }
    
    try {
        throw ProtoculWarning("Warning");
    } catch (const ProtoculWarning& e) {
        caught_warning = true;
    } catch (...) {
        ADD_FAILURE() << "Should catch ProtoculWarning";
    }
    
    EXPECT_TRUE(caught_error);
    EXPECT_TRUE(caught_warning);
}

TEST_F(ErrorHandlingTest, empty_error_message) {
    ProtoculError error("");
    EXPECT_STREQ(error.what(), "");
}

TEST_F(ErrorHandlingTest, long_error_message) {
    std::string long_msg(1000, 'x');
    ProtoculError error(long_msg);
    EXPECT_STREQ(error.what(), long_msg.c_str());
}

TEST_F(ErrorHandlingTest, error_message_with_special_characters) {
    std::string special_msg = "Error with \n\t\r special chars: !@#$%^&*()";
    ProtoculError error(special_msg);
    EXPECT_STREQ(error.what(), special_msg.c_str());
}

TEST_F(ErrorHandlingTest, unicode_error_message) {
    std::string unicode_msg = "Błąd: 中文 العربية русский";
    ProtoculError error(unicode_msg);
    EXPECT_STREQ(error.what(), unicode_msg.c_str());
}