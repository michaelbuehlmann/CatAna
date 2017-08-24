#include "doctest.h"
#include <cmath>
#include <random>

#include <catana/catana.hpp>

// Load data directory
#include <catana/config.hpp>


namespace catana {
  extern std::mt19937 rng;
}

using namespace catana;

const std::string test_data_dir(TEST_DATA_DIR);

TEST_SUITE("io");

TEST_CASE ("testing tophat filter") {
  PointContainer point_container;
  point_container.push_back(Point(0, 0, 0.8));
  point_container.push_back(Point(0, 0, 1.2));
  point_container.push_back(Point(1, 1, 1));
  point_container.push_back(Point(0.5, 0.5, 0.5));
      REQUIRE(4 == point_container.size());

  io::TophatRadialWindowFunctionFilter filter(1.);
  filter(point_container);
      CHECK(2 == point_container.size());
      CHECK(doctest::Approx(0.8) == point_container[0].r);
      CHECK(doctest::Approx(0.8660254038) == point_container[1].r);
}

TEST_CASE ("testing gaussian filter") {
  std::uniform_real_distribution<float> dist(-1000, 1000);
  size_t N = (1 << 24);
  size_t N_exp = N;

  PointContainer point_container;
  point_container.reserve(N);
  for(size_t i = 0; i < N; ++i) {
    point_container.push_back(Point(dist(rng), dist(rng), dist(rng)));
  }
      REQUIRE(N_exp == point_container.size());

  io::TophatRadialWindowFunctionFilter filter_tophat(1000.);
  filter_tophat(point_container);

  N_exp *= 0.5235987756;
      CHECK(point_container.size() == doctest::Approx(N_exp).epsilon(0.1 * N_exp));

  float R0(100.f);
  io::GaussianRadialWindowFunctionFilter filter_gauss(R0);
  filter_gauss(point_container);

  N_exp *= 0.001329340388;
      CHECK(point_container.size() == doctest::Approx(N_exp).epsilon(0.1 * N_exp));
}

TEST_CASE ("testing interpolated gaussian filter") {
  std::uniform_real_distribution<float> dist(-1000, 1000);
  size_t N = (1 << 24);
  size_t N_exp = N;

  PointContainer point_container;
  point_container.reserve(N);
  for(size_t i = 0; i < N; ++i) {
    point_container.push_back(Point(dist(rng), dist(rng), dist(rng)));
  }
      REQUIRE(N_exp == point_container.size());

  io::TophatRadialWindowFunctionFilter filter_tophat(1000.f);
  filter_tophat(point_container);

  N_exp *= 0.5235987756;
      CHECK(point_container.size() == doctest::Approx(N_exp).epsilon(0.1 * N_exp));

  auto window_function = [](double r) { return std::exp(-std::pow(r / 100., 2)); };
  io::GenericRadialWindowFunctionFilter filter_gauss(window_function, 10000, 0, 1000.);
  filter_gauss(point_container);

  N_exp *= 0.001329340388;
      CHECK(point_container.size() == doctest::Approx(N_exp).epsilon(0.1 * N_exp));
}


TEST_CASE ("testing angular mask filter") {
  PointContainer point_container;
  point_container.push_back(point_from_spherical_position(1, 0, -1));  // should be accepted
  point_container.push_back(point_from_spherical_position(1, 0, 1));  // should be removed



  // Map is true almost everywhere, false in the region around (theta=0, phi=1)
  std::string filename = "testmap.fits";

  io::AngularMaskFilter filter(test_data_dir + filename);

  filter(point_container);
      REQUIRE(1 == point_container.size());
      CHECK(doctest::Approx(-1.) == point_container[0].p.phi);
}

TEST_SUITE_END();