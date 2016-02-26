//
// Created by Michael Bühlmann on 08/02/16.
//

#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include <catana/types.hpp>
#include "catana/io_tools/Filter.hpp"

// Load data directory
#include <catana/config.hpp>
#include <catana/iotools.hpp>

using namespace catana;

const std::string test_data_dir(TEST_DATA_DIR);

std::mt19937 rng;

TEST(Filter, TopHat) {
    ObjectContainer object_container;
    object_container.push_back(Object(0,0,0.8));
    object_container.push_back(Object(0,0,1.2));
    object_container.push_back(Object(1,1,1));
    object_container.push_back(Object(0.5,0.5,0.5));
    ASSERT_EQ(4, object_container.size());

    io::TophatRadialWindowFunctionFilter filter(1.);
    filter(object_container);
    EXPECT_EQ(2, object_container.size());
    EXPECT_FLOAT_EQ(0.8, object_container[0].r);
    EXPECT_FLOAT_EQ(0.8660254038, object_container[1].r);
}

TEST(Filter, Gaussian) {
    std::uniform_real_distribution<float> dist(-1000, 1000);
    size_t N = (1 << 24);
    size_t N_exp = N;

    ObjectContainer object_container;
    object_container.reserve(N);
    for(size_t i=0; i<N; ++i) {
        object_container.push_back(Object(dist(rng), dist(rng), dist(rng)));
    }
    ASSERT_EQ(N_exp, object_container.size()) << "Wrong number of objects generated";

    io::TophatRadialWindowFunctionFilter filter_tophat(1000.);
    filter_tophat(object_container);

    N_exp *= 0.5235987756;
    EXPECT_NEAR(object_container.size(), N_exp, 0.1*N_exp);

    float R0(100.f);
    io::GaussianRadialWindowFunctionFilter filter_gauss(R0);
    filter_gauss(object_container);

    N_exp *=0.001329340388;
    EXPECT_NEAR(object_container.size(), N_exp, 0.1*N_exp);
}

TEST(Filter, GaussianInterpolated) {
    std::uniform_real_distribution<float> dist(-1000, 1000);
    size_t N = (1 << 24);
    size_t N_exp = N;

    ObjectContainer object_container;
    object_container.reserve(N);
    for(size_t i=0; i<N; ++i) {
        object_container.push_back(Object(dist(rng), dist(rng), dist(rng)));
    }
    ASSERT_EQ(N_exp, object_container.size()) << "Wrong number of objects generated";

    io::TophatRadialWindowFunctionFilter filter_tophat(1000.f);
    filter_tophat(object_container);

    N_exp *= 0.5235987756;
    EXPECT_NEAR(object_container.size(), N_exp, 0.1*N_exp);

    auto window_function = [](double r){return std::exp(-std::pow(r/100., 2));};
    io::GenericRadialWindowFunctionFilter filter_gauss(window_function, 10000, 0, 1000.);
    filter_gauss(object_container);

    N_exp *=0.001329340388;
    EXPECT_NEAR(object_container.size(), N_exp, 0.1*N_exp);
}



TEST(Filter, AngularMask) {
    ObjectContainer object_container;
    object_container.push_back(object_from_spherical_position(1,0,-1));  // should be accepted
    object_container.push_back(object_from_spherical_position(1,0,1));  // should be removed



    // Map is true almost everywhere, false in the region around (theta=0, phi=1)
    std::string filename = "testmap.fits";

    io::AngularMaskFilter filter(test_data_dir+filename);

    filter(object_container);
    ASSERT_EQ(1, object_container.size());
    EXPECT_FLOAT_EQ(-1, object_container[0].p.phi);
}
