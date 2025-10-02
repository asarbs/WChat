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

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

TEST(MathUtilsTest, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(0, 0), 0);
}

// Test odejmowania
TEST(MathUtilsTest, SubNumbers) {
    EXPECT_EQ(sub(5, 3), 2);
    EXPECT_EQ(sub(3, 5), -2);
}

// Test mieszany
TEST(MathUtilsTest, Mixed) {
    int result = add(sub(10, 3), 2);  // (10-3)+2 = 9
    EXPECT_EQ(result, 9);
}
