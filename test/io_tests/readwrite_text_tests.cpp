#include "doctest.h"
#include <catana/io.hpp>

#include <catana/config.hpp>


using namespace catana;

const std::string test_data_dir(TEST_DATA_DIR);

TEST_SUITE("io");

TEST_CASE ("testing TextSource full reading") {
  io::TextSource<io::CartesianRecord<double>> source(test_data_dir + "mock_data.txt");
  ObjectContainer oc(100);
  auto n = source.read(oc.begin(), 100);
      CHECK(20 == n);

  ObjectContainer oc2(100);
      CHECK(-1 == source.read(oc2.begin(), 100));
  source.reset();
  auto n2 = source.read(oc2.begin(), 100);
      CHECK(n == n2);


      CHECK(doctest::Approx(8.002900041) == oc[0].r);
}

TEST_CASE ("testing TextSource chunck reading") {
  io::TextSource<io::CartesianRecord<double>> source(test_data_dir + "mock_data.txt");
  ObjectContainer oc(100);
  int n = 0;
  auto o_ptr = oc.begin();

  while(n != -1) {
    n = source.read(o_ptr++, 1);
  }
      CHECK(21 == o_ptr - oc.begin());
}


TEST_CASE ("testing TextSink with CartesianRecord") {
  ObjectContainer oc, oc2(100);
  oc.add_object(Object(1, 2, 3));
  oc.add_object(Object(0, 0, 2));
  oc.add_object(Object(-1, -1, -1));
  oc.add_object(Object(0, 0, 0));

  io::TextSink<io::CartesianRecord<double>> sink("test.txt");
  int n = sink.write(oc.begin(), oc.size());
      CHECK(oc.size() == n);

  io::TextSource<io::CartesianRecord<double>> source("test.txt");
  n = source.read(oc2.begin(), 100);
      REQUIRE(oc.size() == n);
  for(int i = 0; i < oc.size(); ++i) {
        CHECK(doctest::Approx(oc[i].r) == oc2[i].r);
        CHECK(doctest::Approx(oc[i].p.theta) == oc2[i].p.theta);
        CHECK(doctest::Approx(oc[i].p.phi) == oc2[i].p.phi);
  }
}

TEST_CASE ("testing TextSink with SphericalRecord") {
  ObjectContainer oc, oc2(100);
  oc.add_object(Object(1, 2, 3));
  oc.add_object(Object(0, 0, 2));
  oc.add_object(Object(-1, -1, -1));
  oc.add_object(Object(0, 0, 0));

  io::TextSink<io::SphericalRecord<double>> sink("test.txt");
  int n = sink.write(oc.begin(), oc.size());
      CHECK(oc.size() == n);

  io::TextSource<io::SphericalRecord<double>> source("test.txt");
  n = source.read(oc2.begin(), 100);
      REQUIRE(oc.size() == n);
  for(int i = 0; i < oc.size(); ++i) {
        CHECK(doctest::Approx(oc[i].r) == oc2[i].r);
        CHECK(doctest::Approx(oc[i].p.theta) == oc2[i].p.theta);
        CHECK(doctest::Approx(oc[i].p.phi) == oc2[i].p.phi);
  }
}

TEST_CASE ("testing TextSink with threedex record") {
  ObjectContainer oc, oc2(100);
  oc.add_object(Object(1, 2, 3));
  oc.add_object(Object(0, 0, 2));
  oc.add_object(Object(-1, -1, -1));
  oc.add_object(Object(0, 0, 0));

  io::TextSink<io::SphericalRecord<double, io::SphericalTextFormat::THREEDEX>> sink("test3dex.txt");
  int n = sink.write(oc.begin(), oc.size());
      CHECK(oc.size() == n);

  io::TextSource<io::SphericalRecord<double, io::SphericalTextFormat::THREEDEX>> source("test3dex.txt");
  n = source.read(oc2.begin(), 100);
      REQUIRE(oc.size() == n);
  for(int i = 0; i < oc.size(); ++i) {
        CHECK(doctest::Approx(oc[i].r).epsilon(1e-12) == oc2[i].r);
        CHECK(doctest::Approx(oc[i].p.theta).epsilon(1e-12) == oc2[i].p.theta);
        CHECK(doctest::Approx(oc[i].p.phi).epsilon(1e-12) == oc2[i].p.phi);
  }
}


// TODO: Move to different place
TEST_CASE ("testing Source to Objectcontainer conversion") {
  io::TextSource<io::CartesianRecord<double>> source(test_data_dir + "mock_data.txt");
  ObjectContainer oc(source.get_objectcontainer());
      CHECK(20 == oc.size());
      CHECK(doctest::Approx(8.002900041) == oc[0].r);
}


TEST_CASE ("testing Source to PixelizedObjectContainer conversion") {
  io::TextSource<io::CartesianRecord<double>> source(test_data_dir + "mock_data.txt");
  PixelizedObjectContainer poc(source.get_pixobjectcontainer(32));
      CHECK(20 == poc.get_nobjects());
      CHECK(32 * 32 * 12 == poc.size());
}

TEST_SUITE_END();