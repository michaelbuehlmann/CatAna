//
// Created by Michael Bühlmann on 04/11/15.
//

#include "gtest/gtest.h"
#include <catana/besseltools.hpp>

using namespace catana;

TEST(BesselZeros, known_zeros){ //MATHEMATICA
    besseltools::SphericalBesselZeros bz(0);
    EXPECT_FLOAT_EQ(3.141592653589791, bz[0]);
    EXPECT_FLOAT_EQ(6.283185307179586, bz[1]);
    EXPECT_FLOAT_EQ(9.42477796076938, bz[2]);

    bz.compute_up_to(100);

    besseltools::SphericalBesselZeros bz2(5);
    EXPECT_NEAR(38.8836, bz2[9], 0.0001);
}