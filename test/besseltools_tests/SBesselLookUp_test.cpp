//
// Created by Michael Bühlmann on 19/02/16.
//

#include <catana/besseltools.hpp>
#include <gtest/gtest.h>

#include <random>
#include <gsl/gsl_sf_bessel.h>
#include <iostream>
#include <algorithm>

#ifndef ALL_TESTS
std::mt19937 rng;
#endif

using namespace catana;

TEST(SBesselLookUp, Accuracy){
    int N_TEST = 1000;

    for(int l=0; l<10; ++l){
        besseltools::SBesselInterpolator sblu(l, 10., 4000);
        std::uniform_real_distribution<double> z_dist(0,sblu.get_zmax());
        for(int i=0; i<N_TEST; ++i){
            double z = z_dist(rng);
            double gsl_jlz = gsl_sf_bessel_jl(l, z);
            EXPECT_NEAR(gsl_jlz, sblu(z), std::max(1e-8, std::abs(0.05*gsl_jlz))) << "More than 5% error at l=" << l << ", z=" << z;
        }
    }
}

TEST(SBesselLookUp, UpToZero){
    int N_TEST = 1000;

    for(int l=0; l<10; ++l){
        besseltools::SBesselInterpolator sblu = besseltools::SBesselInterpolator_uptozero(l, 4, 4000);
        double z_max = sblu.get_zmax();
        EXPECT_NEAR(sblu(z_max), 0, 1e-10);
    }
}