//
// Created by Michael Bühlmann on 03/02/16.
//

#include "gtest/gtest.h"
#include <catana/types.hpp>

using namespace catana;

#ifndef ALL_TESTS
#include <random>
std::mt19937 rng;
#endif //ALL_TESTS

TEST(Object, Creation){
    Object obj1(0,0,1);
    Object obj2(1,1,1);
    Object obj3(-380,-800,200);

    EXPECT_FLOAT_EQ(1, obj1.r);
    EXPECT_FLOAT_EQ(0, obj1.p.theta);
    EXPECT_FLOAT_EQ(0, obj1.p.phi);

    EXPECT_FLOAT_EQ(1.73205081, obj2.r);
    EXPECT_FLOAT_EQ(9.55316618e-01, obj2.p.theta);
    EXPECT_FLOAT_EQ(7.85398163e-01, obj2.p.phi);

    EXPECT_FLOAT_EQ(9.07964757e+02, obj3.r);
    EXPECT_FLOAT_EQ(1.34870213e+00, obj3.p.theta);
    EXPECT_FLOAT_EQ(4.26894064e+00, obj3.p.phi);
}

TEST(ObjectContainer, Subset) {
    size_t N(1<<20);
    ObjectContainer object_container;
    for(size_t i=0; i<N/2; ++i) {
        object_container.add_object(Object(1, 0, 0));
    }
    for(size_t i=0; i<N/2; ++i) {
        object_container.add_object(Object(2, 0, 0));
    }
    ASSERT_EQ(N, object_container.size());

    create_object_container_subset(object_container, N/2);
    ASSERT_EQ(N/2, object_container.size());

    size_t objects_with_r2 = 0;
    std::for_each(object_container.begin(), object_container.end(), [&](Object obj){if(obj.r > 1.5) ++objects_with_r2;});

    EXPECT_NEAR(N/4, objects_with_r2, 0.1*N/4);
}

TEST(PixelizedObjectContainer, Creation) {
    PixelizedObjectContainer pix2(256);
    EXPECT_EQ(12*256*256, pix2.size());

    ASSERT_DEATH(PixelizedObjectContainer pix3(255);, " ");
}

TEST(PixelizedObjectContainer, AddingObjects) {
    PixelizedObjectContainer pix(256);
    Object obj = object_from_spherical_position(10., 1.2, 0.4);
    pix.add_object(obj);
    EXPECT_EQ(1, pix.get_nobjects());

    int found = 0;
    for(int i=0; i<pix.size(); ++i){
        PixelObjects pix_objs = pix[i];
        if(pix_objs.size() > 0) {
            found += pix_objs.size();
            EXPECT_DOUBLE_EQ(pix_objs[0], obj.r);
            EXPECT_NEAR(pix_objs.p.theta, obj.p.theta, 1e-2);
            EXPECT_NEAR(pix_objs.p.phi, obj.p.phi, 1e-2);
        }
    }
    EXPECT_EQ(1, found);
}

TEST(PixelizedObjectContainer, CreateFromObjectContainer) {
    ObjectContainer oc;
    oc.add_object(Object(-1,2,3));
    oc.add_object(Object(-4,1,0));
    oc.add_object(Object(0,1,2));
    oc.add_object(Object(1,4,0));
    oc.add_object(Object(1,2,3));

    PixelizedObjectContainer pix_oc(16, oc);
    EXPECT_EQ(oc.size(), pix_oc.get_nobjects());

}

TEST(PixelizedObjectContainer, CountMap) {
    ObjectContainer oc;
    oc.add_object(Object(-1,2,3));
    oc.add_object(Object(-4,1,0));
    oc.add_object(Object(0,1,2));
    oc.add_object(Object(1,4,0));
    oc.add_object(Object(1,2,3));

    PixelizedObjectContainer pix_oc(16, oc);
    auto countmap = pix_oc.get_countmap();
    ASSERT_EQ(16*16*12, countmap.rows());
    EXPECT_EQ(5, countmap.sum());

}