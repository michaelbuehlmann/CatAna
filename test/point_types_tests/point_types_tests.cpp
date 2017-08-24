//
// Created by Michael Bühlmann on 03/02/16.
//

#include "doctest.h"
#include <catana/catana.hpp>


using namespace catana;

TEST_CASE ("Testing Point creation") {
  Point obj1(0, 0, 1);
  Point obj2(1, 1, 1);
  Point obj3(-380, -800, 200);

  CHECK(obj1.r == doctest::Approx(1));
  CHECK(obj1.p.theta == doctest::Approx(0));
  CHECK(obj1.p.phi == doctest::Approx(0));

  CHECK(obj2.r == doctest::Approx(1.73205081));
  CHECK(obj2.p.theta == doctest::Approx(9.55316618e-01));
  CHECK(obj2.p.phi == doctest::Approx(7.85398163e-01));

  CHECK(obj3.r == doctest::Approx(9.07964757e+02));
  CHECK(obj3.p.theta == doctest::Approx(1.34870213e+00));
  CHECK(obj3.p.phi == doctest::Approx(4.26894064e+00));
}

TEST_CASE ("Testing subset function of PointContainer") {
  size_t N(1 << 20);
  PointContainer point_container;
  for(
  size_t i = 0;
  i < N / 2; ++i) {
    point_container.
    add_point(Point(1, 0, 0));
  }
  for(
  size_t i = 0;
  i < N / 2; ++i) {
    point_container.
    add_point(Point(2, 0, 0));
  }
  REQUIRE(point_container.size() == N);

  create_point_container_subset(point_container, N / 2);
  REQUIRE(point_container.size() == N / 2);

  size_t points_with_r2 = 0;
  std::for_each(point_container.begin(), point_container.end(), [&](Point obj) {
    if(obj.r > 1.5)
      ++points_with_r2;
  });

  CHECK(points_with_r2 == doctest::Approx(N / 4).epsilon(0.1 * N / 4));
}

TEST_CASE ("Testing PixelizedPointContainer creation") {
  PixelizedPointContainer pix2(256);
  CHECK(12 * 256 * 256 == pix2.size());

  CHECK_THROWS(PixelizedPointContainer pix3(255););
}

TEST_CASE ("Testing PixelizedPointContainer adding points") {
  PixelizedPointContainer pix(256);
  Point obj = point_from_spherical_position(10., 1.2, 0.4);
  pix.add_point(obj);
  CHECK(pix.get_npoints() == 1);

  int found = 0;
  for(int i = 0; i < pix.size(); ++i) {
    PixelPoints pix_objs = pix[i];
    if(pix_objs.size() > 0) {
      found += pix_objs.size();

      CHECK(pix_objs[0] == doctest::Approx(obj.r));
      CHECK(pix_objs.p.theta == doctest::Approx(obj.p.theta).epsilon(1e-2));
      CHECK(pix_objs.p.phi == doctest::Approx(obj.p.phi).epsilon(1e-2));
    }
  }
  CHECK(found == 1);
}

TEST_CASE ("Testing PixelizedPointContainer creation from PointContainer") {
  PointContainer oc;
  oc.add_point(Point(-1, 2, 3));
  oc.add_point(Point(-4, 1, 0));
  oc.add_point(Point(0, 1, 2));
  oc.add_point(Point(1, 4, 0));
  oc.add_point(Point(1, 2, 3));

  PixelizedPointContainer pix_oc(16, oc);
  CHECK(oc.size() == pix_oc.get_npoints());
}

TEST_CASE ("Testing PixelizedPointContainer CountMap") {
  PointContainer oc;
  oc.add_point(Point(-1, 2, 3));
  oc.add_point(Point(-4, 1, 0));
  oc.add_point(Point(0, 1, 2));
  oc.add_point(Point(1, 4, 0));
  oc.add_point(Point(1, 2, 3));

  PixelizedPointContainer pix_oc(16, oc);
  auto countmap = pix_oc.get_countmap();

  REQUIRE(16 * 16 * 12 == countmap.rows());
  CHECK(5 == countmap.sum());
}