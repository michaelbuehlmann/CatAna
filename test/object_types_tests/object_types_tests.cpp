//
// Created by Michael Bühlmann on 03/02/16.
//

#include "gtest/gtest.h"
#include <catana/types.hpp>


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
        object_container.push_back(Object(1, 0, 0));
    }
    for(size_t i=0; i<N/2; ++i) {
        object_container.push_back(Object(2, 0, 0));
    }
    ASSERT_EQ(N, object_container.size());

    create_object_container_subset(object_container, N/2);
    ASSERT_EQ(N/2, object_container.size());

    size_t objects_with_r2 = 0;
    std::for_each(object_container.begin(), object_container.end(), [&](Object obj){if(obj.r > 1.5) ++objects_with_r2;});

    EXPECT_NEAR(N/4, objects_with_r2, 0.1*N/4);
}