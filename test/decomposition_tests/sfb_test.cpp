#include "doctest.h"
#include <catana/io.hpp>
#include <catana/decomposition.hpp>

using namespace catana;

// LOAD TEST_DATA_DIR
#include <catana/config.hpp>


const std::string test_data_dir(TEST_DATA_DIR);

Eigen::ArrayXXd read_python_cln(int lmax, int nmax) {
  std::ifstream f(test_data_dir + "gaussian_catalog.python.c_ln", std::ios::in);
  Eigen::ArrayXXd c_ln(lmax, nmax);
  int i = 0;
  for(std::string line; std::getline(f, line);) {
    std::istringstream iss(line);
    int j = 0;
    double cln;
    while(iss >> cln) {
      c_ln(i, j++) = cln;
    }
    assert(j == nmax);
    ++i;
  }
  assert(i == lmax);
  return c_ln;
}


TEST_CASE ("testing SFB raw method") {
  ObjectContainer object_container;
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::ObjectContainerSink sink(object_container);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }
      REQUIRE(445 == object_container.size());
  int lmax = 10;
  int nmax = 10;

  // Compare with Python
  auto python_cln = read_python_cln(lmax, nmax);

  KClkk kclkk = _sfb_raw(object_container, lmax, nmax, 2500, 5535184878.03, false, false, false, false);
  kclkk.savetxt("sfb_raw");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.01 * python_cln(l, n)));
    }
  }

  // With interpolation
  kclkk = _sfb_raw(object_container, lmax, nmax, 2500, 5535184878.03, false, false, false, true);
  kclkk.savetxt("sfb_raw_inter");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.01 * python_cln(l, n)));
    }
  }
}

TEST_CASE ("testing SFB raw method with parallelization") {
  ObjectContainer object_container;
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::ObjectContainerSink sink(object_container);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }
  int lmax = 10;
  int nmax = 10;

  // Compare with Python
  auto python_cln = read_python_cln(lmax, nmax);


  KClkk kclkk = _sfb_raw(object_container, lmax, nmax, 2500, 5535184878.03, false, false, true, false);
  kclkk.savetxt("sfb_raw_p");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.01 * python_cln(l, n)));
    }
  }

  kclkk = _sfb_raw(object_container, lmax, nmax, 2500, 5535184878.03, false, false, true, true);
  kclkk.savetxt("sfb_raw_p_inter");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.01 * python_cln(l, n)));
    }
  }
}


TEST_CASE ("testing SFB reverse method") {
  PixelizedObjectContainer pix_obj_cont(64);
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::PixelizedObjectContainerSink sink(pix_obj_cont);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }

  int lmax = 10;
  int nmax = 10;

  // Compare with Python
  auto python_cln = read_python_cln(lmax, nmax);

  KClkk kclkk = _sfb_reverse(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, false, false);
  kclkk.savetxt("sfb_reverse");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }

  kclkk = _sfb_reverse(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, false, true);
  kclkk.savetxt("sfb_reverse_inter");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }
}

TEST_CASE ("testing SFB reverse method with parallelization") {
  PixelizedObjectContainer pix_obj_cont(64);
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::PixelizedObjectContainerSink sink(pix_obj_cont);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }

  int lmax = 10;
  int nmax = 10;

  // Compare with Python
  auto python_cln = read_python_cln(lmax, nmax);

  KClkk kclkk = _sfb_reverse(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, true, false);
  kclkk.savetxt("sfb_reverse_p");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }

  kclkk = _sfb_reverse(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, true, true);
  kclkk.savetxt("sfb_reverse_p_inter");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }
}

TEST_CASE ("testing SFB pixelized method") {
  PixelizedObjectContainer pix_obj_cont(64);
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::PixelizedObjectContainerSink sink(pix_obj_cont);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }

  int lmax = 10;
  int nmax = 10;

  // Compare with Python
  auto python_cln = read_python_cln(lmax, nmax);

  KClkk kclkk = _sfb_reverse_fft(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, false, false);
  kclkk.savetxt("sfb_reverse_fft");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }

  kclkk = _sfb_reverse_fft(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, false, true);
  kclkk.savetxt("sfb_reverse_fft_inter");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }
}

TEST_CASE ("testing SFB pixelized method with parallelization") {
  PixelizedObjectContainer pix_obj_cont(64);
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::PixelizedObjectContainerSink sink(pix_obj_cont);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }

  int lmax = 10;
  int nmax = 10;

  // Compare with Python
  auto python_cln = read_python_cln(lmax, nmax);

  KClkk kclkk = _sfb_reverse_fft(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, true, false);
  kclkk.savetxt("sfb_reverse_fft_p");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }

  kclkk = _sfb_reverse_fft(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, true, true);
  kclkk.savetxt("sfb_reverse_fft_p_inter");

  for(int l = 0; l < lmax; ++l) {
    for(int n = 0; n < nmax; ++n) {
          CHECK(kclkk.c_ln(l, n) == doctest::Approx(python_cln(l, n)).epsilon(0.1 * python_cln(l, n)));
    }
  }
}


TEST_CASE ("testing SFB f_lmn computation") {
  ObjectContainer object_container;
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::ObjectContainerSink sink(object_container);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }


  PixelizedObjectContainer pix_obj_cont(64);
  {
    io::TextSource<io::CartesianRecord<float>> source(test_data_dir + "gaussian_catalog.txt", 1, 0);
    io::PixelizedObjectContainerSink sink(pix_obj_cont);
    io::FilterStream fs(&source, &sink, 10000, false);
    fs.run();
  }

  int lmax = 10;
  int nmax = 10;

  auto kclkk_raw = _sfb_raw(object_container, lmax, nmax, 2500, 5535184878.03, true, false, true, true);
      REQUIRE(lmax == kclkk_raw.f_lmn.size());
  auto kclkk_rev = _sfb_reverse(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, true, false, true, true);
      REQUIRE(lmax == kclkk_rev.f_lmn.size());
  auto kclkk_rev_fft = _sfb_reverse_fft(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, true, false, true, true);
      REQUIRE(lmax == kclkk_rev_fft.f_lmn.size());

  for(int l = 0; l < lmax; ++l) {
        CHECK(kclkk_rev.f_lmn[l].isApprox(kclkk_rev.f_lmn[l]));
  }
}