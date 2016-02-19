//
// Created by Michael Bühlmann on 19/02/16.
//

#include <cmath>
#include "gtest/gtest.h"
#include <catana/besseltools.hpp>

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