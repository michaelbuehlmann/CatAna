//
// Created by Michael Bühlmann on 03/02/16.
//

#include "gtest/gtest.h"
#include "catana/tools/object_types.hpp"

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