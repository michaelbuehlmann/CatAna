//
// Created by Michael Bühlmann on 14/02/16.
//

#include <catana/io.hpp>
#include <catana/types.hpp>
#include "gtest/gtest.h"

using namespace catana;

#ifndef ALL_TESTS
#include<random>
std::mt19937 rng;
#endif //ALL_TESTS

TEST(RawData, IO) {
    ObjectContainer oc;
    oc.push_back(object_from_spherical_position(1., 1. , 3.));
    oc.push_back(object_from_spherical_position(2., 0.1 , 0.));
    oc.push_back(object_from_spherical_position(3., 0.2 , 1.));
    oc.push_back(object_from_spherical_position(100., 0.3 , 4.));

    std::string filename = "RawIOTest.dat";
    using record_t = io::CartesianRecord<float>;
    int n;

    // Writing
    io::RawBinarySink<record_t> sink(filename, false);
    n = sink.write(oc.begin(), oc.size());
    EXPECT_EQ(oc.size(), n);
    sink.close();
    n = sink.write(oc.begin(), oc.size());
    EXPECT_EQ(-1, n);

    // Reading
    ObjectContainer oc2(2*oc.size());
    int read = 0;
    n = 0;
    io::RawBinarySource<record_t> source(filename, false);
    do {
        read += n;
        ASSERT_LE(read, oc.size()) << "Read too many records!!";
        n = source.read(oc2.begin()+read, 1);
    } while(n != -1);
    EXPECT_EQ(oc.size(), read);
    oc2.resize(read);

    for(int i=0; i<oc.size(); ++i){
        EXPECT_FLOAT_EQ(oc[i].r, oc2[i].r) << "at i=="<<i;
        EXPECT_FLOAT_EQ(oc[i].p.theta, oc2[i].p.theta) << "at i=="<<i;
        EXPECT_FLOAT_EQ(oc[i].p.phi, oc2[i].p.phi) << "at i=="<<i;
    }
}


TEST(RawData, IOLarge) {
    ObjectContainer oc;
    for(size_t i=0; i<(1<<12); ++i){
        oc.push_back(object_from_spherical_position(1000./i, 1., 2.));
    }

    std::string filename = "RawIOLargeTest.dat";
    using record_t = io::SphericalRecord<float>;
    long long int n;

    // Writing
    io::RawBinarySink<record_t> sink(filename, false);
    n = sink.write(oc.begin(), oc.size());


    // Reading
    Object buffer[100];
    size_t read = 0;
    n = 0;
    io::RawBinarySource<record_t> source(filename, false);

    do {
        read += n;
        n = source.read(buffer, 100);
    } while(n != -1);
    EXPECT_EQ(oc.size(), read);

    // Assert that it returns -1
    ASSERT_EQ(-1, source.read(buffer, 100));

    // Check if reset works
    source.reset();
    read = n = 0;
    do {
        read += n;
        n = source.read(buffer, 100);
    } while(n != -1);
    EXPECT_EQ(oc.size(), read);

}