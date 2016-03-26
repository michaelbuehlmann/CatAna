//
// Created by Michael Bühlmann on 23/02/16.
//

#include <gtest/gtest.h>
#include <catana/iotools.hpp>

#include <catana/config.hpp>

using namespace catana;

const std::string test_data_dir(TEST_DATA_DIR);

#ifndef ALL_TESTS
#include<random>
std::mt19937 rng;
#endif //ALL_TESTS


TEST(ReadTxt, AllAndReset){
    io::TextSource<io::CartesianRecord<double>> source(test_data_dir+"mock_data.txt");
    ObjectContainer oc(100);
    auto n = source.read(oc.begin(), 100);
    EXPECT_EQ(20, n);

    ObjectContainer oc2(100);
    EXPECT_EQ(-1, source.read(oc2.begin(), 100));
    source.reset();
    auto n2 = source.read(oc2.begin(), 100);
    EXPECT_EQ(n, n2);



    EXPECT_FLOAT_EQ(8.002900041, oc[0].r);
}

TEST(ReadTxt, SmallChuncks){
    io::TextSource<io::CartesianRecord<double>> source(test_data_dir+"mock_data.txt");
    ObjectContainer oc(100);
    int n=0;
    auto o_ptr = oc.begin();

    while(n!=-1)
    {
        n = source.read(o_ptr++, 1);
    }
    EXPECT_EQ(21, o_ptr-oc.begin());
}


TEST(WriteTxt, Cartesian){
    ObjectContainer oc, oc2(100);
    oc.add_object(Object(1,2,3));
    oc.add_object(Object(0,0,2));
    oc.add_object(Object(-1,-1,-1));
    oc.add_object(Object(0,0,0));

    io::TextSink<io::CartesianRecord<double>> sink("test.txt");
    int n = sink.write(oc.begin(), oc.size());
    EXPECT_EQ(oc.size(), n);

    io::TextSource<io::CartesianRecord<double>> source("test.txt");
    n = source.read(oc2.begin(), 100);
    ASSERT_EQ(oc.size(), n);
    for(int i=0; i<oc.size(); ++i){
        EXPECT_DOUBLE_EQ(oc[i].r, oc2[i].r);
        EXPECT_DOUBLE_EQ(oc[i].p.theta, oc2[i].p.theta);
        EXPECT_DOUBLE_EQ(oc[i].p.phi, oc2[i].p.phi);
    }
}

TEST(WriteTxt, Spherical){
    ObjectContainer oc, oc2(100);
    oc.add_object(Object(1,2,3));
    oc.add_object(Object(0,0,2));
    oc.add_object(Object(-1,-1,-1));
    oc.add_object(Object(0,0,0));

    io::TextSink<io::SphericalRecord<double>> sink("test.txt");
    int n = sink.write(oc.begin(), oc.size());
    EXPECT_EQ(oc.size(), n);

    io::TextSource<io::SphericalRecord<double>> source("test.txt");
    n = source.read(oc2.begin(), 100);
    ASSERT_EQ(oc.size(), n);
    for(int i=0; i<oc.size(); ++i){
        EXPECT_DOUBLE_EQ(oc[i].r, oc2[i].r);
        EXPECT_DOUBLE_EQ(oc[i].p.theta, oc2[i].p.theta);
        EXPECT_DOUBLE_EQ(oc[i].p.phi, oc2[i].p.phi);
    }
}

TEST(WriteTxt, Spherical3DEX){
    ObjectContainer oc, oc2(100);
    oc.add_object(Object(1,2,3));
    oc.add_object(Object(0,0,2));
    oc.add_object(Object(-1,-1,-1));
    oc.add_object(Object(0,0,0));

    io::TextSink<io::SphericalRecord<double, io::SphericalTextFormat::THREEDEX>> sink("test3dex.txt");
    int n = sink.write(oc.begin(), oc.size());
    EXPECT_EQ(oc.size(), n);

    io::TextSource<io::SphericalRecord<double, io::SphericalTextFormat::THREEDEX>> source("test3dex.txt");
    n = source.read(oc2.begin(), 100);
    ASSERT_EQ(oc.size(), n);
    for(int i=0; i<oc.size(); ++i){
        EXPECT_NEAR(oc[i].r, oc2[i].r, 1e-12);
        EXPECT_NEAR(oc[i].p.theta, oc2[i].p.theta, 1e-12);
        EXPECT_NEAR(oc[i].p.phi, oc2[i].p.phi, 1e-12);
    }
}


// TODO: Move to different place
TEST(Source, ToObjectContainer){
    io::TextSource<io::CartesianRecord<double>> source(test_data_dir+"mock_data.txt");
    ObjectContainer oc(source.get_objectcontainer());
    EXPECT_EQ(20, oc.size());
    EXPECT_FLOAT_EQ(8.002900041, oc[0].r);
}


TEST(Source, ToPixelizedObjectContainer){
    io::TextSource<io::CartesianRecord<double>> source(test_data_dir+"mock_data.txt");
    PixelizedObjectContainer poc(source.get_pixobjectcontainer(32));
    EXPECT_EQ(20, poc.get_nobjects());
    EXPECT_EQ(32*32*12, poc.size());
}