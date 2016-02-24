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
    EXPECT_EQ(512, input.get_nobjects());

    ObjectContainer oc(input.get_nobjects());

    auto nrecords = input.read(oc.begin(), 20);
    EXPECT_EQ(20, nrecords);
}


TEST(ReadHDF5, FullFile){
    std::string filename = "mock_data.hdf5";
    std::string dataset_name = "/table";
    ObjectContainer oc;

    oc = read_hdf5_positions<CartesianRecord<float>>(test_data_dir + filename, dataset_name, 0.7, 0, false);
    ASSERT_EQ(512, oc.size());
    for(int i=0; i<test_positions_hdf.size(); ++i){
        EXPECT_FLOAT_EQ(test_radius_hdf[i], oc[test_positions_hdf[i]].r);
        EXPECT_FLOAT_EQ(test_theta_hdf[i], oc[test_positions_hdf[i]].p.theta);
        EXPECT_FLOAT_EQ(test_phi_hdf[i], oc[test_positions_hdf[i]].p.phi);
    }
}

