//
// Created by Michael Bühlmann on 04/11/15.
//

#include <cmath>
#include "gtest/gtest.h"
#include "catana/tools/besseltools.hpp"


class BesselZerosTest : public ::testing::Test {
protected:
    BesselZeros bz0{0};
    BesselZeros bz1{1};
    BesselZeros bz55_1{5.5};
    BesselZeros bz55_2{5.5};
};

TEST(BesselZeros, known_zeros){ //MATHEMATICA
    BesselZeros bz(0);
    EXPECT_DOUBLE_EQ(2.4048255576957727686, bz[0]);
    EXPECT_DOUBLE_EQ(5.5200781102863106496, bz[1]);
    EXPECT_DOUBLE_EQ(8.6537279129110122170, bz[2]);

    bz.compute_up_to(100);

    BesselZeros bz2(5.5);
    EXPECT_NEAR(38.8836, bz2[9], 0.0001);
}

TEST(DoubleBesselIntegration, basic_tests){
    auto w = [](double r){return std::exp(-std::pow(r/40.,2)) * std::pow(r,2);};
    EXPECT_DOUBLE_EQ(double_sbessel_integrator(w,6,100,2.3,4.5), double_sbessel_integrator(w,6,100,4.5,2.3));
}

TEST(DoubleBesselIntegration, known_values){ //MATHEMATICA
    auto w = [](double r){return 1.;};
    EXPECT_NEAR(1.56582, double_sbessel_integrator(w,0,100,1,1), 0.0001);
    EXPECT_NEAR(0.785393, double_sbessel_integrator(w,0,100,1,2), 0.00001);
    EXPECT_NEAR(0.000133933, double_sbessel_integrator(w,4,100,1,4.2), 0.000000001);
}