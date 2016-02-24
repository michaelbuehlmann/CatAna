//
// Created by Michael Bühlmann on 23/02/16.
//

#include <gtest/gtest.h>
#include <catana/iotools.hpp>

#include <catana/config.hpp>
const std::string test_data_dir(TEST_DATA_DIR);

#ifndef ALL_TESTS
#include<random>
std::mt19937 rng;
#endif //ALL_TESTS


TEST(ReadTxt, Read1){
    TextSource<CartesianRecord<double>> source(test_data_dir+"mock_data.txt");
    ObjectContainer oc(100);
    auto n = source.read(oc.begin(), 100);
    EXPECT_EQ(20, n);

    n = source.read(oc.begin(), 100);
    EXPECT_EQ(-1, n);

    EXPECT_FLOAT_EQ(19.49477717, oc[0].r);
}

TEST(ReadTxt, Read2){
    TextSource<CartesianRecord<double>> source(test_data_dir+"mock_data.txt");
    ObjectContainer oc(100);
    int n=0;
    auto o_ptr = oc.begin();

    while(n!=-1)
    {
        n = source.read(o_ptr++, 1);
    }
    EXPECT_EQ(21, o_ptr-oc.begin());
}