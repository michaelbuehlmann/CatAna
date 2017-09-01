#include "doctest.h"
#include <catana/catana.hpp>


using namespace catana;

TEST_SUITE("io");

TEST_CASE ("testing PixelizedPointContainerSink") {
  PointContainer oc;
  for(size_t i = 1; i < (1 << 12); ++i) {
    oc.push_back(point_from_spherical_position(1., 3. / i, 6. / i));
  }

  PixelizedPointContainer pix_oc(256);

  io::PixelizedPointContainerSink pix_sink(pix_oc);
  long int n = pix_sink.write(oc.begin(), oc.size());
      CHECK(oc.size() == n);
      CHECK(oc.size() == pix_oc.get_npoints());
}

TEST_SUITE_END();