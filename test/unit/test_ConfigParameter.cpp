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
#include "server/core/ConfigParameter.h"

using namespace WChat::ChatServer::core;

TEST(ConfigParameter, CreateParameter) {
    ConfigParameter cp("PramName", "Param Description", "15");
    EXPECT_EQ(cp.as<int>(), 15);
    EXPECT_EQ(cp.name(), "PramName");
    EXPECT_EQ(cp.description(), "Param Description");
}

TEST(ConfigParameter, InvalidParameterName) {
    EXPECT_THROW({ ConfigParameter cp("Pram Name", "Param Description", "15"); }, std::invalid_argument);
}
