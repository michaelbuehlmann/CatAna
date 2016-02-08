//
// Created by Michael Bühlmann on 08/02/16.
//

#include "gtest/gtest.h"
#include "catana/io_tools/write_hdf5.hpp"
#include "catana/io_tools/read_hdf5.hpp"

TEST(SmallSink, Cartesian) {
    std::string filename = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/sdss_lpicola/test_0.hdf";
    std::string dataset_name = "/tpart";

    ObjectContainer oc;
    oc.push_back(Object(0,0,1));
    oc.push_back(Object(1,1,1));
    oc.push_back(Object(100,1,100));

    {
        HDF5Sink<CartesianRecord<float>> hdf5_sink(filename, dataset_name, 1, 0, true, false);
        auto records = hdf5_sink.write(oc.begin(), oc.size());
        EXPECT_EQ(oc.size(), records);
    }

    HDF5Source<CartesianRecord<float>> hdf5_source(filename, dataset_name, 1, 0, true);
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

TEST(SmallSink, Spherical) {
    std::string filename = "/Users/michael/Documents/ETH/2015HS/MasterThesis/Data/sdss_lpicola/test_1.hdf";
    std::string dataset_name = "/tpart";

    ObjectContainer oc;
    oc.push_back(Object(0,0,1));
    oc.push_back(Object(1,1,1));
    oc.push_back(Object(100,1,100));

    {
        HDF5Sink<SphericalRecord<float>> hdf5_sink(filename, dataset_name, 1, 0, true, false);
        auto records = hdf5_sink.write(oc.begin(), oc.size());
        EXPECT_EQ(oc.size(), records);
    }

    HDF5Source<SphericalRecord<float>> hdf5_source(filename, dataset_name, 1, 0, false);
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