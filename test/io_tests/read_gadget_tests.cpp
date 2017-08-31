#include "doctest.h"
#include <catana/catana.hpp>


using namespace catana;

// Load data directory
#include <catana/config.hpp>


const std::string test_data_dir(TEST_DATA_DIR);

// Some known values:
std::vector<unsigned int> test_positions = {0, 10, 100, 511};
std::vector<float> test_radius = {11.43271463, 7.47124219, 5.63280499, 8.62107958};
std::vector<float> test_theta = {0.90536372, 1.8781445, 1.42947137, 0.87425136};
std::vector<float> test_phi = {4.04660937, 2.79354124, 2.96859039, 5.80961737};

TEST_SUITE("io");

TEST_CASE ("testing GadgetSource with partial data") {
  std::string filename = "mock_data_z0p000.0";

  io::GadgetSource input(test_data_dir + filename, false);

  Point points[20];
  long returned_values;
  returned_values = input.read(points, 20);

      CHECK(20 == returned_values);
}

TEST_CASE ("testing GadgetSource with all data") {
  std::string filename = "mock_data_z0p000.0";
  io::GadgetSource input(test_data_dir + filename, false);

  Point tmp_points[10];
  PointContainer oc;

  long returned_values = 0;
  do {
    std::copy(&tmp_points[0], &tmp_points[returned_values], std::back_inserter(oc));
    returned_values = input.read(tmp_points, 10);
  } while(returned_values != -1);
      CHECK(512 == oc.size());


  filename = "mock_data_z0p000.1";
  io::GadgetSource input2(test_data_dir + filename, false);
  PointContainer oc2;
  returned_values = 0;
  do {
    std::copy(&tmp_points[0], &tmp_points[returned_values], std::back_inserter(oc2));
    returned_values = input2.read(tmp_points, 10);
  } while(returned_values != -1);
      CHECK(512 == oc2.size());

  for(int i = 0; i < test_positions.size(); ++i) {
        CHECK(doctest::Approx(test_radius[i]*0.7) == oc[test_positions[i]].r);
        CHECK(doctest::Approx(test_theta[i]) == oc[test_positions[i]].p.theta);
        CHECK(doctest::Approx(test_phi[i]) == oc[test_positions[i]].p.phi);
  }

  for(int i = 0; i < test_positions.size(); ++i) {
        CHECK(doctest::Approx(test_radius[i]*0.7) == oc2[test_positions[i]].r);
        CHECK(doctest::Approx(test_theta[i]) == oc2[test_positions[i]].p.theta);
        CHECK(doctest::Approx(test_phi[i]) == oc2[test_positions[i]].p.phi);
  }

  // Assert that nothing left
  auto n_points = input.read(tmp_points, 10);
      REQUIRE(-1 == n_points);

  // Test Reset
  PointContainer oc3(1000);
  input.reset();
  n_points = input.read(oc3.begin(), 1000);
      REQUIRE(512 == n_points);
}

TEST_SUITE_END();