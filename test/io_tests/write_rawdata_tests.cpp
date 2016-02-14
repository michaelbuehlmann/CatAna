//
// Created by Michael Bühlmann on 14/02/16.
//

#include <catana/iotools.hpp>
#include <catana/types.hpp>
#include "gtest/gtest.h"

TEST(RawData, IO) {
    ObjectContainer oc;
    oc.push_back(object_from_spherical_position(1., 1. , 3.));
    oc.push_back(object_from_spherical_position(2., 0.1 , 0.));
    oc.push_back(object_from_spherical_position(3., 0.2 , 1.));
    oc.push_back(object_from_spherical_position(100., 0.3 , 4.));

    std::string filename = "RawIOTest.dat";
    using record_t = CartesianRecord<float>;
    int n;

    // Writing
    RawBinarySink<record_t> sink(filename, false);
    n = sink.write(oc.begin(), oc.size());
    EXPECT_EQ(oc.size(), n);
    sink.close();
    n = sink.write(oc.begin(), oc.size());
    EXPECT_EQ(-1, n);

    // Reading
    ObjectContainer oc2(2*oc.size());
    int read = 0;
    n = 0;
    RawBinarySource<record_t> source(filename, false);
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