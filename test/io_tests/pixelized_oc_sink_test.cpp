#include "doctest.h"
#include <catana/types.hpp>
#include <catana/io.hpp>


using namespace catana;

TEST_SUITE("io");

TEST_CASE ("testing PixelizedObjectContainerSink") {
  ObjectContainer oc;
  for(size_t i = 1; i < (1 << 12); ++i) {
    oc.push_back(object_from_spherical_position(1., 3. / i, 6. / i));
  }

  PixelizedObjectContainer pix_oc(256);

  io::PixelizedObjectContainerSink pix_sink(pix_oc);
  long int n = pix_sink.write(oc.begin(), oc.size());
      CHECK(oc.size() == n);
      CHECK(oc.size() == pix_oc.get_nobjects());
}

TEST_SUITE_END();