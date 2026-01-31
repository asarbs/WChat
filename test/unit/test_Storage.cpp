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
#include "server/core/storage/Storage.h"
#include "server/core/storage/StorageFactory.h"

using namespace WChat::ChatServer::core::storage;

class StorageTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Use Volatile storage for testing (in-memory) via factory
            storage = getStorage();
        }

        void TearDown() override {
            storage->clean();
        }

        std::shared_ptr<Storage> storage;
};

TEST_F(StorageTest, add_user_success) {
    std::string username      = "testuser";
    std::string password_hash = "hashed_password";
    auto user_id_opt          = storage->addUser(username, password_hash);

    ASSERT_TRUE(user_id_opt.has_value());
    uint64_t user_id = user_id_opt.value();
    EXPECT_GT(user_id, 0);
    EXPECT_EQ(storage->getUserIdByName(username, password_hash).value(), user_id);
}

TEST_F(StorageTest, add_duplicate_user) {
    std::string username      = "testuser";
    std::string password_hash = "hashed_password";
    auto first_id_opt         = storage->addUser(username, password_hash);
    auto second_id_opt        = storage->addUser(username, password_hash);

    ASSERT_TRUE(first_id_opt.has_value());
    ASSERT_TRUE(second_id_opt.has_value());
    // Should return the same ID for duplicate user
    EXPECT_EQ(first_id_opt.value(), second_id_opt.value());
}

TEST_F(StorageTest, add_user_with_different_passwords) {
    std::string username  = "testuser";
    std::string password1 = "password1";
    std::string password2 = "password2";
    auto first_id_opt     = storage->addUser(username, password1);
    auto second_id_opt    = storage->addUser(username, password2);

    ASSERT_TRUE(first_id_opt.has_value());
    ASSERT_TRUE(second_id_opt.has_value());
    // Different passwords might create different users or return existing user
    // Implementation dependent behavior
}

TEST_F(StorageTest, get_nonexistent_user_by_name) {
    std::string username      = "nonexistent";
    std::string password_hash = "password_hash";
    auto user_id              = storage->getUserIdByName(username, password_hash);

    EXPECT_FALSE(user_id.has_value());
}

TEST_F(StorageTest, register_user_success) {
    auto user_id_opt = storage->addUser("testuser", "hashed_password");
    ASSERT_TRUE(user_id_opt.has_value());
    uint64_t user_id = user_id_opt.value();
    bool registered  = storage->registerUser(user_id);

    EXPECT_TRUE(registered);
    EXPECT_TRUE(storage->isUserRegistered(user_id));
}

TEST_F(StorageTest, register_nonexistent_user) {
    uint64_t nonexistent_id = 999999;
    bool registered         = storage->registerUser(nonexistent_id);

    EXPECT_FALSE(registered);
}

TEST_F(StorageTest, unregister_user_success) {
    auto user_id_opt = storage->addUser("testuser", "hashed_password");
    ASSERT_TRUE(user_id_opt.has_value());
    uint64_t user_id = user_id_opt.value();
    storage->registerUser(user_id);
    bool unregistered = storage->unregister(user_id);

    EXPECT_TRUE(unregistered);
    EXPECT_FALSE(storage->isUserRegistered(user_id));
}

TEST_F(StorageTest, unregister_nonexistent_user) {
    uint64_t nonexistent_id = 999999;
    bool unregistered       = storage->unregister(nonexistent_id);

    EXPECT_FALSE(unregistered);
}

TEST_F(StorageTest, save_and_retrieve_message) {
    auto user_id_opt = storage->addUser("testuser", "hashed_password");
    ASSERT_TRUE(user_id_opt.has_value());
    uint64_t user_id = user_id_opt.value();
    storage->registerUser(user_id);

    bool saved = storage->saveMsg(user_id, 1, "Hello World", false);
    EXPECT_TRUE(saved);
    EXPECT_TRUE(storage->hasMsg(user_id));
}

TEST_F(StorageTest, pop_message_success) {
    auto user_id_opt = storage->addUser("testuser", "hashed_password");
    ASSERT_TRUE(user_id_opt.has_value());
    uint64_t user_id = user_id_opt.value();
    storage->registerUser(user_id);
    storage->saveMsg(user_id, 1, "Test Message", false);

    auto msg = storage->popMsg(user_id);
    EXPECT_EQ(msg.from, 1);
    EXPECT_EQ(msg.message, "Test Message");
    EXPECT_FALSE(storage->hasMsg(user_id));  // Message should be removed
}

TEST_F(StorageTest, create_connection_success) {
    auto user1_id_opt = storage->addUser("user1", "password1");
    auto user2_id_opt = storage->addUser("user2", "password2");
    ASSERT_TRUE(user1_id_opt.has_value());
    ASSERT_TRUE(user2_id_opt.has_value());
    uint64_t user1_id = user1_id_opt.value();
    uint64_t user2_id = user2_id_opt.value();

    bool created = storage->createConnection(user1_id, user2_id);
    EXPECT_TRUE(created);

    auto contacts = storage->getContacts(user1_id);
    EXPECT_GT(contacts.size(), 0);
}

TEST_F(StorageTest, create_connection_nonexistent_users) {
    uint64_t user1_id = 999999;
    uint64_t user2_id = 1000000;

    bool created = storage->createConnection(user1_id, user2_id);
    EXPECT_FALSE(created);
}

TEST_F(StorageTest, get_contacts_empty) {
    auto user_id_opt = storage->addUser("testuser", "hashed_password");
    ASSERT_TRUE(user_id_opt.has_value());
    uint64_t user_id = user_id_opt.value();
    auto contacts    = storage->getContacts(user_id);

    EXPECT_EQ(contacts.size(), 0);
}

TEST_F(StorageTest, get_contacts_with_connections) {
    auto user1_id_opt = storage->addUser("user1", "password1");
    auto user2_id_opt = storage->addUser("user2", "password2");
    ASSERT_TRUE(user1_id_opt.has_value());
    ASSERT_TRUE(user2_id_opt.has_value());
    uint64_t user1_id = user1_id_opt.value();
    uint64_t user2_id = user2_id_opt.value();

    storage->createConnection(user1_id, user2_id);
    auto contacts = storage->getContacts(user1_id);

    EXPECT_GT(contacts.size(), 0);
}

TEST_F(StorageTest, clean_storage) {
    auto user_id_opt = storage->addUser("testuser", "hashed_password");
    ASSERT_TRUE(user_id_opt.has_value());
    uint64_t user_id = user_id_opt.value();
    storage->registerUser(user_id);
    storage->saveMsg(user_id, 1, "Test Message", false);

    storage->clean();

    EXPECT_FALSE(storage->isUserRegistered(user_id));
    EXPECT_FALSE(storage->hasMsg(user_id));
}
