//
// Created by Michael Bühlmann on 03/02/16.
//

#include <catana/types.hpp>
#include "catana/iotools.hpp"
#include "gtest/gtest.h"

#include <string>

// Load data directory
#include <catana/config.hpp>
const std::string test_data_dir(TEST_DATA_DIR);

#ifndef ALL_TESTS
#include<random>
std::mt19937 rng;
#endif //ALL_TESTS


// Some known values:
std::vector<unsigned int> test_positions_hdf = {0, 10, 100, 511};
std::vector<float> test_radius_hdf = {11.43271463, 7.47124219, 5.63280499, 8.62107958};
std::vector<float> test_theta_hdf = {0.90536372, 1.8781445 , 1.42947137, 0.87425136};
std::vector<float> test_phi_hdf = {4.04660937, 2.79354124, 2.96859039, 5.80961737};



TEST(ReadHDF5, Stream){
    std::string filename = "mock_data.hdf5";
    std::string dataset_name = "/table";

    HDF5Source<CartesianRecord<float>> input(test_data_dir + filename, dataset_name, 0.7, 0, false);
    auto total_objects = input.get_nobjects();
    EXPECT_EQ(512, total_objects);

    ObjectContainer oc(total_objects);

    auto nrecords = input.read(oc.begin(), 20);
    EXPECT_EQ(20, nrecords);
    nrecords += input.read(oc.begin()+nrecords, 1000);
    EXPECT_EQ(total_objects, nrecords);

    ObjectContainer oc2(input.get_nobjects());
    auto nrecords2 = input.read(oc2.begin(), 1000);
    ASSERT_EQ(-1, nrecords2);
    input.reset();
    nrecords2 = input.read(oc2.begin(), 1000);
    EXPECT_EQ(nrecords, nrecords2);
}

