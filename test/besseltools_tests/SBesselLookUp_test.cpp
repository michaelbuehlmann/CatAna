//
// Created by Michael Bühlmann on 19/02/16.
//

#include <catana/besseltools.hpp>
#include <gtest/gtest.h>

#include <random>
#include <boost/math/special_functions/bessel.hpp>
#include <iostream>
#include <algorithm>

#ifndef ALL_TESTS
std::mt19937 rng;
#endif

using namespace catana;

TEST(SBesselLookUp, Accuracy){
    int N_TEST = 1000;

    for(int l=0; l<10; ++l){
        besseltools::SBesselInterpolator sblu(l, 4, 4000);
        std::uniform_real_distribution<double> z_dist(0,sblu.get_zmax());
        for(int i=0; i<N_TEST; ++i){
            double z = z_dist(rng);
            double boost_jlz = boost::math::sph_bessel(l, z);
            EXPECT_NEAR(boost_jlz, sblu(z), std::max(1e-8, std::abs(0.05*boost_jlz))) << "More than 5% error at l=" << l << ", z=" << z;
        }
    }
}