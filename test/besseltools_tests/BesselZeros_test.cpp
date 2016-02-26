//
// Created by Michael Bühlmann on 04/11/15.
//

#include "gtest/gtest.h"
#include <catana/besseltools.hpp>

using namespace catana;

class BesselZerosTest : public ::testing::Test {
protected:
    besseltools::BesselZeros bz0{0};
    besseltools::BesselZeros bz1{1};
    besseltools::BesselZeros bz55_1{5.5};
    besseltools::BesselZeros bz55_2{5.5};
};

TEST(BesselZeros, known_zeros){ //MATHEMATICA
    besseltools::BesselZeros bz(0);
    EXPECT_DOUBLE_EQ(2.4048255576957727686, bz[0]);
    EXPECT_DOUBLE_EQ(5.5200781102863106496, bz[1]);
    EXPECT_DOUBLE_EQ(8.6537279129110122170, bz[2]);

    bz.compute_up_to(100);

    besseltools::BesselZeros bz2(5.5);
    EXPECT_NEAR(38.8836, bz2[9], 0.0001);
}