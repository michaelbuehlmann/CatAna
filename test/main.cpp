//
// Created by Michael Bühlmann on 09/02/16.
//

#include "gtest/gtest.h"
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}