#include "doctest.h"
#include <catana/catana.hpp>

// Load data directory
#include <catana/config.hpp>


const std::string test_data_dir(TEST_DATA_DIR);
using namespace catana;

TEST_SUITE("io");

TEST_CASE ("testing FilterStream with small tophat") {
  PointContainer point_container;
  point_container.push_back(Point(0, 0, 0.8));
  point_container.push_back(Point(0, 0, 1.2));
  point_container.push_back(Point(1, 1, 1));
  point_container.push_back(Point(0.5, 0.5, 0.5));

  PointContainer point_container_result;

  io::PointContainerSource source(point_container);
  io::PointContainerSink sink(point_container_result);

  io::FilterStream filter_stream(&source, &sink, 10, false);
  io::TophatRadialWindowFunctionFilter filter(1.);
  filter_stream.add_filter(&filter);
  filter_stream.run();

      CHECK(2 == point_container_result.size());
      CHECK(doctest::Approx(0.8) == point_container_result[0].r);
      CHECK(doctest::Approx(0.8660254038) == point_container_result[1].r);
}


TEST_CASE ("testing FilterStream with large tophat") {
  PointContainer point_container;
  int N = 1 << 11;
  for(int i = 0; i < N; ++i) {
    point_container.push_back(point_from_spherical_position((2.f * i) / N, 0.f, 0.f));
  }

  PointContainer point_container_result;

  io::PointContainerSource source(point_container);
  io::PointContainerSink sink(point_container_result);

  io::FilterStream filter_stream(&source, &sink, 100, false);
  io::TophatRadialWindowFunctionFilter filter(1.);
  filter_stream.add_filter(&filter);
  filter_stream.run();

      CHECK(point_container_result.size() == doctest::Approx(N / 2.).epsilon(1));
}

TEST_CASE ("testing FilterStream with subsample") {
  PointContainer point_container;
  int N = 1 << 11;
  for(int i = 0; i < N; ++i) {
    point_container.push_back(point_from_spherical_position((2.f * i) / N, 0.f, 0.f));
  }

  PointContainer point_container_result;
  io::PointContainerSource source(point_container);
  io::PointContainerSink sink(point_container_result);

  io::FilterStream filter_stream(&source, &sink, 100, false);
  filter_stream.run(1 << 10, TEST_DATA_DIR + std::string("tmp.dat"));

      CHECK((1 << 10) == point_container_result.size());
}

TEST_CASE ("testing FilterStream with multiple inputs") {
  PointContainer point_container;
  point_container.push_back(Point(0, 0, 0.8));
  point_container.push_back(Point(0, 0, 1.2));
  point_container.push_back(Point(1, 1, 1));
  point_container.push_back(Point(0.5, 0.5, 0.5));

  PointContainer point_container_result;

  io::PointContainerSource source1(point_container);
  io::PointContainerSource source2(point_container);
  io::PointContainerSink sink(point_container_result);

  io::FilterStream filter_stream(&source1, &sink, 10, false);
  io::TophatRadialWindowFunctionFilter filter(1.);
  filter_stream.add_filter(&filter);
  filter_stream.run_totemp("tmp.bin", false);
  filter_stream.set_source(&source2);
  filter_stream.run_totemp("tmp.bin", true);

  filter_stream.run_fromtemp("tmp.bin", 0, true);
      CHECK(4 == point_container_result.size());
      CHECK(0.8 == point_container_result[0].r);
      CHECK(doctest::Approx(0.8) == point_container_result[2].r);
      CHECK(doctest::Approx(0.8660254038) == point_container_result[1].r);
      CHECK(doctest::Approx(0.8660254038) == point_container_result[3].r);
}

TEST_SUITE_END();