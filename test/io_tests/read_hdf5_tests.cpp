//
// Created by Michael Bühlmann on 03/02/16.
//

#include "catana/tools/object_types.hpp"
#include "catana/iotools.hpp"
#include "gtest/gtest.h"

#include <string>
TEST(ReadHDF5, Stream){
    std::string filename = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/sdss_lpicola/catalog_i0.hdf";
    std::string dataset_name = "/particles_t0";

    HDF5Source<CartesianRecord<float>> input(filename, dataset_name, 0.7, 0, false);
    EXPECT_EQ(68435327, input.get_nobjects());

    ObjectContainer oc(input.get_nobjects());

    auto nrecords = input.read(oc.begin(), 8000001);
    EXPECT_EQ(8000001, nrecords);
    oc.resize(nrecords);

    // Some known values:
    std::vector<unsigned int> test_positions = {0, 2000000, 8000000};
    std::vector<float> test_radius = {2.57592505e+03, 2.41338110e+03, 1.76010010e+03};
    std::vector<float> test_theta = {1.56444418e+00, 1.86860752e+00, 1.04656744e+00};
    std::vector<float> test_phi = {2.11752248e+00, 2.12997460e+00, 2.18058467e+00};

    for(int i=0; i<test_positions.size(); ++i){
        EXPECT_FLOAT_EQ(test_radius[i], oc[test_positions[i]].r) << "At index " << test_positions[i];
        EXPECT_FLOAT_EQ(test_theta[i], oc[test_positions[i]].p.theta) << "At index " << test_positions[i];
        EXPECT_FLOAT_EQ(test_phi[i], oc[test_positions[i]].p.phi) << "At index " << test_positions[i];
    }
}


TEST(ReadHDF5, FullFile){
    std::string filename = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/sdss_lpicola/catalog_i0.hdf";
    std::string dataset_name = "/particles_t0";
    ObjectContainer oc;

    oc = read_hdf5_positions<CartesianRecord<float>>(filename, dataset_name, 0.7, 0, false);

    // Some known values:
    std::vector<unsigned int> test_positions = {0, 2000000, 8000000};
    std::vector<float> test_radius = {2.57592505e+03, 2.41338110e+03, 1.76010010e+03};
    std::vector<float> test_theta = {1.56444418e+00, 1.86860752e+00, 1.04656744e+00};
    std::vector<float> test_phi = {2.11752248e+00, 2.12997460e+00, 2.18058467e+00};

    for(int i=0; i<test_positions.size(); ++i){
        EXPECT_FLOAT_EQ(test_radius[i], oc[test_positions[i]].r);
        EXPECT_FLOAT_EQ(test_theta[i], oc[test_positions[i]].p.theta);
        EXPECT_FLOAT_EQ(test_phi[i], oc[test_positions[i]].p.phi);
    }
}

