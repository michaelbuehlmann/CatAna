//
// Created by Michael Bühlmann on 15/02/16.
//

#include <gtest/gtest.h>
#include <catana/tools/return_types.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <fstream>
#include <string>
#include <sstream>

TEST(ReturnTypes, KCLKK){
    short lmax=10;
    short nmax=20;
    double rmax=5.4;
    KClkk kclkk(lmax, nmax, rmax);

    ASSERT_EQ(kclkk.k_ln.rows(), lmax);
    ASSERT_EQ(kclkk.k_ln.cols(), nmax);

    ASSERT_EQ(kclkk.c_ln.rows(), lmax);
    ASSERT_EQ(kclkk.c_ln.cols(), nmax);

    for(int l=0; l<lmax; ++l) {
        EXPECT_NEAR(boost::math::sph_bessel(l, kclkk.k_ln(l, nmax-1)*rmax), 0, 1e-16) << "l=" << l;
        EXPECT_EQ(0, kclkk.c_ln(l,nmax-1)); // Initialized to 0?
    }

    kclkk.savetxt("test");

    // Some helper variables
    int line_count = 0;
    int word_count;
    double nbr;
    std::istringstream iss;

    std::ifstream fs_kln("test.k_ln");
    ASSERT_TRUE(fs_kln.good());
    for(std::string line; std::getline(fs_kln, line); ){
        iss = std::istringstream(line);
        word_count = 0;
        while(iss>>nbr) {
            EXPECT_FLOAT_EQ(kclkk.k_ln(line_count, word_count), nbr) << "l=" << line_count << " n=" << word_count;
            ++word_count;
        }
        EXPECT_EQ(nmax, word_count) << "l=" << line_count;
        ++line_count;
    }
    EXPECT_EQ(line_count, lmax);
    fs_kln.close();

    std::ifstream fs_cln("test.c_ln");
    ASSERT_TRUE(fs_cln.good());
    line_count = 0;
    for(std::string line; std::getline(fs_cln, line); ){
        iss = std::istringstream(line);
        word_count = 0;
        while(iss>>nbr) {
            EXPECT_FLOAT_EQ(kclkk.c_ln(line_count, word_count), nbr) << "l=" << line_count << " n=" << word_count;
            ++word_count;
        }
        EXPECT_EQ(nmax, word_count) << "l=" << line_count;
        ++line_count;
    }
    EXPECT_EQ(line_count, lmax);
    fs_cln.close();

}