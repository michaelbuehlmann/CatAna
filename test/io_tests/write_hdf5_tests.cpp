//
// Created by Michael Bühlmann on 08/02/16.
//

#include "gtest/gtest.h"
#include <catana/io_tools/sinks/HDF5Sink.hpp>
#include <catana/io_tools/sources/HDF5Source.hpp>

using namespace catana;

// Load data directory
#include <catana/config.hpp>
const std::string test_data_dir(TEST_DATA_DIR);

#ifndef ALL_TESTS
#include<random>
std::mt19937 rng;
#endif //ALL_TESTS

TEST(HDF5SmallSink, Cartesian) {
    std::string filename = "write_test_1.hdf5";
    std::string dataset_name = "/tpart";

    ObjectContainer oc;
    oc.push_back(Object(0,0,1));
    oc.push_back(Object(1,1,1));
    oc.push_back(Object(100,1,100));

    {
        io::HDF5Sink<io::CartesianRecord<float>> hdf5_sink(test_data_dir + filename, dataset_name, 1, 0, true, false);
        auto records = hdf5_sink.write(oc.begin(), oc.size());
        EXPECT_EQ(oc.size(), records);
    }

    io::HDF5Source<io::CartesianRecord<float>> hdf5_source(test_data_dir + filename, dataset_name, 1, 0, false);
    EXPECT_EQ(oc.size(), hdf5_source.get_nobjects());
    ObjectContainer oc_read(hdf5_source.get_nobjects());

    auto records = hdf5_source.read(oc_read.begin(), hdf5_source.get_nobjects());
    EXPECT_EQ(oc.size(), records);

    for(int i=0; i<oc.size(); ++i){
        EXPECT_FLOAT_EQ(oc[i].r, oc_read[i].r) << " for index " << i;
        EXPECT_FLOAT_EQ(oc[i].p.theta, oc_read[i].p.theta) << " for index " << i;
        EXPECT_FLOAT_EQ(oc[i].p.phi, oc_read[i].p.phi) << " for index " << i;
    }
}

TEST(HDF5SmallSink, Spherical) {
    std::string filename = "write_test_2.hdf5";
    std::string dataset_name = "/tpart";

    ObjectContainer oc;
    oc.push_back(Object(0,0,1));
    oc.push_back(Object(1,1,1));
    oc.push_back(Object(100,1,100));

    {
        io::HDF5Sink<io::SphericalRecord<float>> hdf5_sink(test_data_dir + filename, dataset_name, 1, 0, true, false);
        auto records = hdf5_sink.write(oc.begin(), oc.size());
        EXPECT_EQ(oc.size(), records);
    }

    io::HDF5Source<io::SphericalRecord<float>> hdf5_source(test_data_dir + filename, dataset_name, 1, 0, false);
    EXPECT_EQ(oc.size(), hdf5_source.get_nobjects());
    ObjectContainer oc_read(hdf5_source.get_nobjects());

    auto records = hdf5_source.read(oc_read.begin(), hdf5_source.get_nobjects());
    EXPECT_EQ(oc.size(), records);

    for(int i=0; i<oc.size(); ++i){
        EXPECT_FLOAT_EQ(oc[i].r, oc_read[i].r) << " for index " << i;
        EXPECT_FLOAT_EQ(oc[i].p.theta, oc_read[i].p.theta) << " for index " << i;
        EXPECT_FLOAT_EQ(oc[i].p.phi, oc_read[i].p.phi) << " for index " << i;
    }
}