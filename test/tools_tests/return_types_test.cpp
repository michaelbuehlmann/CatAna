//
// Created by Michael Bühlmann on 15/02/16.
//

#include "doctest.h"
#include <catana/decomposition/return_types.hpp>
#include <gsl/gsl_sf_bessel.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace catana;

TEST_CASE("Testing return types"){
    short lmax=10;
    short nmax=20;
    double rmax=5.4;
    KClkk kclkk(lmax, nmax, rmax);

    REQUIRE(kclkk.k_ln.rows() == lmax);
    REQUIRE(kclkk.k_ln.cols() == nmax);

    REQUIRE(kclkk.c_ln.rows() == lmax);
    REQUIRE(kclkk.c_ln.cols() == nmax);

    for(int l=0; l<lmax; ++l) {
      CHECK(gsl_sf_bessel_jl(l, kclkk.k_ln(l, nmax-1)*rmax) == doctest::Approx(0).epsilon(1e-15));
      CHECK(kclkk.c_ln(l, nmax-1) == 0);
    }

    kclkk.savetxt("test");

    // Some helper variables
    int line_count = 0;
    int word_count;
    double nbr;

    std::ifstream fs_kln("test.k_ln");
    REQUIRE(fs_kln.good());
    for(std::string line; std::getline(fs_kln, line); ){
        std::istringstream iss(line);
        word_count = 0;
        while(iss>>nbr) {
            CHECK(kclkk.k_ln(line_count, word_count) == doctest::Approx(nbr));
            ++word_count;
        }
        CHECK(nmax == word_count);
        ++line_count;
    }
    CHECK(line_count == lmax);
    fs_kln.close();

    std::ifstream fs_cln("test.c_ln");
    REQUIRE(fs_cln.good());
    line_count = 0;
    for(std::string line; std::getline(fs_cln, line); ){
        std::istringstream iss(line);
        word_count = 0;
        while(iss>>nbr) {
            CHECK(kclkk.c_ln(line_count, word_count) == doctest::Approx(nbr));
            ++word_count;
        }
        CHECK(nmax == word_count);
        ++line_count;
    }
    CHECK(line_count == lmax);
    fs_cln.close();

}
