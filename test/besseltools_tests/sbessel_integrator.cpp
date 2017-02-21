#include "doctest.h"
#include <cmath>
#include <catana/besseltools.hpp>


using namespace catana;

TEST_CASE ("testing double_sbessel_integrator") {
  auto w = [](double r) { return std::exp(-std::pow(r / 40., 2)) * std::pow(r, 2); };
  CHECK(
  besseltools::double_sbessel_integrator(w, 6, 100, 2.3, 4.5) ==
  doctest::Approx(besseltools::double_sbessel_integrator(w, 6, 100, 4.5, 2.3))
  );
}

TEST_CASE ("testing known values for double_sbessel_integrator") { //MATHEMATICA
  auto w = [](double r) { return 1.; };
  CHECK(besseltools::double_sbessel_integrator(w, 0, 100, 1, 1) == doctest::Approx(1.56582).epsilon(0.0001));
  CHECK(besseltools::double_sbessel_integrator(w, 0, 100, 1, 2) == doctest::Approx(0.785393).epsilon(0.00001));
  CHECK(besseltools::double_sbessel_integrator(w, 4, 100, 1, 4.2) == doctest::Approx(0.000133933).epsilon(0.000000001));
}