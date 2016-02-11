//
// Created by Michael Bühlmann on 02/02/16.
//

#include <catana/types.hpp>
#include "catana/io_tools/read_gadget.hpp"
#include "gtest/gtest.h"

#include <string>
#include <boost/iostreams/stream_buffer.hpp>
#include <algorithm>


// Load data directory
#include "config.hpp"
const std::string test_data_dir(TEST_DATA_DIR);


// Some known values:
std::vector<unsigned int> test_positions = {0, 10, 100, 511};
std::vector<float> test_radius = {11.43271463, 7.47124219, 5.63280499, 8.62107958};
std::vector<float> test_theta = {0.90536372, 1.8781445 , 1.42947137, 0.87425136};
std::vector<float> test_phi = {4.04660937, 2.79354124, 2.96859039, 5.80961737};


TEST(ReadGadgetStream, SmallBunch) {
    std::string filename = "mock_data_z0p000.0";

    GadgetSource input(test_data_dir + filename, false);

    Object objects[20];
    long returned_values;
    returned_values = input.read(objects, 20);

    EXPECT_EQ(20, returned_values);
}

TEST(ReadGadgetStream, AllObjects) {
    std::string filename = "mock_data_z0p000.0";
    GadgetSource input(test_data_dir + filename, false);

    Object tmp_objects[10];
    ObjectContainer oc;

    long returned_values = 0;
    do {
        std::copy(&tmp_objects[0], &tmp_objects[returned_values], std::back_inserter(oc));
        returned_values = input.read(tmp_objects, 10);
    } while(returned_values != -1);
    EXPECT_EQ(512, oc.size());


    filename = "mock_data_z0p000.1";
    GadgetSource input2(test_data_dir + filename, false);
    ObjectContainer oc2;
    returned_values = 0;
    do {
            std::copy(&tmp_objects[0], &tmp_objects[returned_values], std::back_inserter(oc2));
            returned_values = input2.read(tmp_objects, 10);
    } while(returned_values != -1);
    EXPECT_EQ(512, oc2.size());

    for(int i=0; i<test_positions.size(); ++i){
        EXPECT_FLOAT_EQ(test_radius[i], oc[test_positions[i]].r);
        EXPECT_FLOAT_EQ(test_theta[i], oc[test_positions[i]].p.theta);
        EXPECT_FLOAT_EQ(test_phi[i], oc[test_positions[i]].p.phi);
    }

    for(int i=0; i<test_positions.size(); ++i){
        EXPECT_FLOAT_EQ(test_radius[i], oc2[test_positions[i]].r);
        EXPECT_FLOAT_EQ(test_theta[i], oc2[test_positions[i]].p.theta);
        EXPECT_FLOAT_EQ(test_phi[i], oc2[test_positions[i]].p.phi);
    }
}

TEST(ReadGadget, AllObjects) {
    std::string filename = "mock_data_z0p000.0";
    ObjectContainer oc = read_gadget_halo_positions(test_data_dir + filename, false);
    EXPECT_EQ(512, oc.size());

    for(int i=0; i<test_positions.size(); ++i){
        EXPECT_FLOAT_EQ(test_radius[i], oc[test_positions[i]].r);
        EXPECT_FLOAT_EQ(test_theta[i], oc[test_positions[i]].p.theta);
        EXPECT_FLOAT_EQ(test_phi[i], oc[test_positions[i]].p.phi);

    }
}
