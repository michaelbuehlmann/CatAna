//
// Created by Michael Bühlmann on 02/02/16.
//

#include "catana/tools/object_types.hpp"
#include "catana/io_tools/read_gadget.hpp"
#include "gtest/gtest.h"

#include <string>
#include <boost/iostreams/stream_buffer.hpp>
#include <algorithm>


TEST(ReadGadgetStream, SmallBunch) {
    std::string filename = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/lpicola/test2/output/testrun_z0p000.0";
    GadgetSource input(filename, false);

    Object objects[100];
    long returned_values;
    returned_values = input.read(objects, 100);

    EXPECT_EQ(100, returned_values);
}

TEST(ReadGadgetStream, AllObjects) {
    std::string filename = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/lpicola/test2/output/testrun_z0p000.0";
    GadgetSource input(filename, false);

    Object tmp_objects[10000];
    ObjectContainer oc;

    long returned_values = 0;
    do {
        std::copy(&tmp_objects[0], &tmp_objects[returned_values], std::back_inserter(oc));
        returned_values = input.read(tmp_objects, 10000);
    } while(returned_values != -1);
    EXPECT_EQ(16777216, oc.size());


    filename = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/lpicola/test2/output/testrun_z0p000.3";
    input = GadgetSource(filename, false);
    ObjectContainer oc2;
    returned_values = 0;
    do {
            std::copy(&tmp_objects[0], &tmp_objects[returned_values], std::back_inserter(oc2));
            returned_values = input.read(tmp_objects, 10000);
    } while(returned_values != -1);
    EXPECT_EQ(16777216, oc2.size());

    // Some known values:
    std::vector<unsigned int> test_positions = {1000, 50000, 8000000, 16777215};
    std::vector<float> test_radius = {856.05079375,  863.4106594 ,  473.76745457,  504.04686746};
    std::vector<float> test_theta = {1.29380338,  1.89415404,  1.66455411,  1.04426485};
    std::vector<float> test_phi = {4.23396134,  2.6544129 ,  1.67019825,  0.57489863};

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
    std::string file = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/lpicola/test2/output/testrun_z0p000.0";
    ObjectContainer oc = read_gadget_halo_positions(file, false);
    EXPECT_EQ(16777216, oc.size());
    EXPECT_EQ(0, file.compare("/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/lpicola/test2/output/testrun_z0p000.0"));

    // Some known values:
    std::vector<unsigned int> test_positions = {1000, 50000, 8000000, 16777215};
    std::vector<float> test_radius = {856.05079375,  863.4106594 ,  473.76745457,  504.04686746};
    std::vector<float> test_theta = {1.29380338,  1.89415404,  1.66455411,  1.04426485};
    std::vector<float> test_phi = {4.23396134,  2.6544129 ,  1.67019825,  0.57489863};

    for(int i=0; i<test_positions.size(); ++i){
        EXPECT_FLOAT_EQ(test_radius[i], oc[test_positions[i]].r);
        EXPECT_FLOAT_EQ(test_theta[i], oc[test_positions[i]].p.theta);
        EXPECT_FLOAT_EQ(test_phi[i], oc[test_positions[i]].p.phi);

    }
}