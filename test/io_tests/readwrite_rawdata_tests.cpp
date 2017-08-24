#include "doctest.h"
#include <catana/catana.hpp>


using namespace catana;

TEST_SUITE("io");

TEST_CASE ("testing RawBinary sink and source") {
  PointContainer oc;
  oc.push_back(point_from_spherical_position(1., 1., 3.));
  oc.push_back(point_from_spherical_position(2., 0.1, 0.));
  oc.push_back(point_from_spherical_position(3., 0.2, 1.));
  oc.push_back(point_from_spherical_position(100., 0.3, 4.));

  std::string filename = "RawIOTest.dat";
  using record_t = io::CartesianRecord<float>;
  int n;

  // Writing
  {
    io::RawBinarySink<record_t> sink(filename, false, false);
    n = sink.write(oc.begin(), oc.size());
        CHECK(oc.size() == n);
    sink.close();
    n = sink.write(oc.begin(), oc.size());
        CHECK(-1 == n);
  }

  // Appending
  {
    io::RawBinarySink<record_t> sink(filename, false, true);
    n = sink.write(oc.begin(), oc.size());
        CHECK(oc.size() == n);
    sink.close();
    n = sink.write(oc.begin(), oc.size());
        CHECK(-1 == n);
  }

  // Reading
  PointContainer oc2(10 * oc.size());
  int read = 0;
  n = 0;
  io::RawBinarySource<record_t> source(filename, false);
  do {
    read += n;
        CHECK(read <= 2 * oc.size());
    n = source.read(oc2.begin() + read, 1);
  } while(n != -1);
      CHECK(2 * oc.size() == read);
  oc2.resize(read);

  for(int i = 0; i < 2 * oc.size(); ++i) {
        CHECK(doctest::Approx(oc[i % oc.size()].r) == oc2[i].r);
        CHECK(doctest::Approx(oc[i % oc.size()].p.theta) == oc2[i].p.theta);
        CHECK(doctest::Approx(oc[i % oc.size()].p.phi) == oc2[i].p.phi);
  }
}


TEST_CASE ("testing RawBinary sink and source with large data") {
  PointContainer oc;
  for(size_t i = 0; i < (1 << 12); ++i) {
    oc.push_back(point_from_spherical_position(1000. / i, 1., 2.));
  }

  std::string filename = "RawIOLargeTest.dat";
  using record_t = io::SphericalRecord<float>;
  long long int n;

  // Writing
  io::RawBinarySink<record_t> sink(filename, false);
  n = sink.write(oc.begin(), oc.size());


  // Reading
  Point buffer[100];
  size_t read = 0;
  n = 0;
  io::RawBinarySource<record_t> source(filename, false);

  do {
    read += n;
    n = source.read(buffer, 100);
  } while(n != -1);
      CHECK(oc.size() == read);

  // Assert that it returns -1
      REQUIRE(-1 == source.read(buffer, 100));

  // Check if reset works
  source.reset();
  read = n = 0;
  do {
    read += n;
    n = source.read(buffer, 100);
  } while(n != -1);
      CHECK(oc.size() == read);
}

TEST_SUITE_END();