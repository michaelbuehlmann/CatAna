//
// Created by Michael Bühlmann on 19/02/16.
//

#include "doctest.h"
#include <catana/besseltools.hpp>

#include <random>
#include <gsl/gsl_sf_bessel.h>
#include <iostream>
#include <algorithm>

extern std::mt19937 rng;
using namespace catana;

TEST_CASE ("Testing SBesselLookUp accuracy") {
  int N_TEST = 1000;

  for(unsigned l = 0; l < 10; ++l) {
    besseltools::SBesselInterpolator sblu(l, 10., 4000);
    std::uniform_real_distribution<double> z_dist(0, sblu.get_zmax());
    for(int i = 0; i < N_TEST; ++i) {
      double z = z_dist(rng);
      double gsl_jlz = gsl_sf_bessel_jl(l, z);
      CHECK(sblu(z) == doctest::Approx(gsl_jlz).epsilon(std::max(1e-8, std::abs(0.05 * gsl_jlz))));
    }
  }
}

TEST_CASE ("Testing SBesselLookup uptozero") {
  int N_TEST = 1000;

  for(unsigned l = 0; l < 10; ++l) {
    besseltools::SBesselInterpolator sblu = besseltools::SBesselInterpolator_uptozero(l, 4, 4000);
    double z_max = sblu.get_zmax();
    CHECK(sblu(z_max-1e-10) == doctest::Approx(0).epsilon(1e-8));
  }
}