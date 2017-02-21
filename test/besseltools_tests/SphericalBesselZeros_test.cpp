#include "doctest.h"
#include <catana/besseltools.hpp>


using namespace catana;

TEST_CASE ("Testing SphericalBesselZeros known zeros") { //MATHEMATICA
  besseltools::SphericalBesselZeros bz(0);
  CHECK(bz[0] == doctest::Approx(3.141592653589791));
  CHECK(bz[1] == doctest::Approx(6.283185307179586));
  CHECK(bz[2] == doctest::Approx(9.42477796076938));
  bz.compute_up_to(100);

  besseltools::SphericalBesselZeros bz2(5);
  CHECK(bz2[9] == doctest::Approx(38.8836).epsilon(0.0001));
}