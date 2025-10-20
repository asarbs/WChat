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

#include <tuple>

#include "gtest/gtest.h"
#include "server/core/Config.h"
#include "server/core/ConfigParameter.h"

using namespace WChat::ChatServer::core;

enum class ParamKey {
    Host,
    Port
};

using TestConfigType = Config<ParamKey>;

class ConfigTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Każdy test zaczyna z czystym singletonem
            TestConfigType::instance().clear();
        }

        void TearDown() override {
            TestConfigType::instance().clear();
        }
};

TEST_F(ConfigTest, RegisterStringParam) {
    TestConfigType::instance().addParam(ParamKey::Host, {"host", "Host of server", "lockalhost"});
    const ConfigParameter& param = TestConfigType::instance().get(ParamKey::Host);
    EXPECT_EQ(param.name(), "host");
    EXPECT_EQ(param.description(), "Host of server");
    EXPECT_EQ(std::get<std::string>(param.value()), "lockalhost");
    EXPECT_EQ(TestConfigType::instance().value<std::string>(ParamKey::Host), "lockalhost");
    EXPECT_EQ(TestConfigType::instance().value<std::string>(ParamKey::Host), std::get<std::string>(param.value()));
}

TEST_F(ConfigTest, RegisterIntParam) {
    TestConfigType::instance().addParam(ParamKey::Port, {"port", "Port of server", 9002});
    const ConfigParameter& param = TestConfigType::instance().get(ParamKey::Port);
    EXPECT_EQ(param.name(), "port");
    EXPECT_EQ(param.description(), "Port of server");
    EXPECT_EQ(std::get<int>(param.value()), 9002);
    EXPECT_EQ(TestConfigType::instance().value<int>(ParamKey::Port), 9002);
    EXPECT_EQ(TestConfigType::instance().value<int>(ParamKey::Port), std::get<int>(param.value()));
}

TEST_F(ConfigTest, DoubleRegisterStringParam) {
    TestConfigType::instance().addParam(ParamKey::Host, {"host", "Host of server", "lockalhost1"});

    ConfigParameter cp   = {"host", "Host of server", "lockalhost2"};
    bool exceptionThrown = false;
    try {
        TestConfigType::instance().addParam(ParamKey::Host, cp);
    } catch (TestConfigType::ParameterAlreadyRegistered& e) {
        exceptionThrown = true;
    }

    EXPECT_TRUE(exceptionThrown) << "Expected ParameterAlreadyRegistered exception was not thrown";
}
