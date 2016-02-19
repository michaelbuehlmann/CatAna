//
// Created by Michael Bühlmann on 16/02/16.
//

#include <gtest/gtest.h>
#include <catana/iotools.hpp>
#include <catana/decomposition/sfb_decomposition.hpp>

#include <fstream>
#include <sstream>

// Provide rng (GCC otherwise complains when linking iotools library)
#ifndef ALL_TESTS
#include <random>
std::mt19937 rng;
#endif //ALL_TESTS

// LOAD TEST_DATA_DIR
#include <catana/config.hpp>
const std::string test_data_dir(TEST_DATA_DIR);

Eigen::ArrayXXd read_python_cln(int lmax, int nmax){
    std::ifstream f(test_data_dir+"gaussian_catalog.python.c_ln", std::ios::in);
    Eigen::ArrayXXd c_ln(lmax, nmax);
    int i=0;
    for(std::string line; std::getline(f, line); ){
        std::istringstream iss(line);
        int j=0;
        double cln;
        while(iss>>cln) {
            c_ln(i, j++) = cln;
        }
        assert(j==nmax);
        ++i;
    }
    assert(i==lmax);
    return c_ln;
}



TEST(SFB, Raw) {
    ObjectContainer object_container;
    {
        HDF5Source<CartesianRecord<float>> source(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false);
        ObjectContainerSink sink(object_container);
        FilterStream fs(&source, &sink, 10000, false);
        fs.run();
    }

    int lmax = 10;
    int nmax = 10;

    // Compare with Python
    auto python_cln = read_python_cln(lmax,nmax);

    KClkk kclkk = _decomp_SFB(object_container, lmax, nmax, 2500, 5535184878.03, false, false, false);
    kclkk.savetxt("sfb_raw");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            ASSERT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.01*python_cln(l,n))
                                    << "Error larger than 1% at l="<<l<<", n="<<n<<std::endl;
        }
    }

    // With interpolation
    kclkk = _decomp_SFB(object_container, lmax, nmax, 2500, 5535184878.03, false, false, true);
    kclkk.savetxt("sfb_raw_inter");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            ASSERT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.01*python_cln(l,n))
                                    << "Error larger than 1% at l="<<l<<", n="<<n<<std::endl;
        }
    }
}

TEST(SFB, RawParallel) {
    ObjectContainer object_container;
    {
        HDF5Source<CartesianRecord<float>> source(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false);
        ObjectContainerSink sink(object_container);
        FilterStream fs(&source, &sink, 10000, false);
        fs.run();
    }
    int lmax = 10;
    int nmax = 10;

    // Compare with Python
    auto python_cln = read_python_cln(lmax,nmax);


    KClkk kclkk = _decomp_SFB(object_container, lmax, nmax, 2500, 5535184878.03, false, true, false);
    kclkk.savetxt("sfb_raw_p");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            ASSERT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.01*python_cln(l,n))
                                    << "Error larger than 1% at l="<<l<<", n="<<n<<std::endl;
        }
    }

    kclkk = _decomp_SFB(object_container, lmax, nmax, 2500, 5535184878.03, false, true, true);
    kclkk.savetxt("sfb_raw_p_inter");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            ASSERT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.01*python_cln(l,n))
                                    << "Error larger than 1% at l="<<l<<", n="<<n<<std::endl;
        }
    }
}



TEST(SFB, Reverse) {
    PixelizedObjectContainer pix_obj_cont(64);
    {
        HDF5Source<CartesianRecord<float>> source(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false);
        PixelizedObjectContainerSink sink(pix_obj_cont);
        FilterStream fs(&source, &sink, 10000, false);
        fs.run();
    }

    int lmax = 10;
    int nmax = 10;

    // Compare with Python
    auto python_cln = read_python_cln(lmax,nmax);

    KClkk kclkk = _decomp_SFB(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, false);
    kclkk.savetxt("sfb_reverse");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                                    << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }

    kclkk = _decomp_SFB(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, true);
    kclkk.savetxt("sfb_reverse_inter");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                            << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }
}

TEST(SFB, ReverseParallel) {
    PixelizedObjectContainer pix_obj_cont(64);
    {
        HDF5Source<CartesianRecord<float>> source(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false);
        PixelizedObjectContainerSink sink(pix_obj_cont);
        FilterStream fs(&source, &sink, 10000, false);
        fs.run();
    }

    int lmax = 10;
    int nmax = 10;

    // Compare with Python
    auto python_cln = read_python_cln(lmax,nmax);

    KClkk kclkk = _decomp_SFB(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, true, false);
    kclkk.savetxt("sfb_reverse_p");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                                    << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }

    kclkk = _decomp_SFB(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, true, true);
    kclkk.savetxt("sfb_reverse_p_inter");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                            << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }
}

TEST(SFB, ReverseFFT) {
    PixelizedObjectContainer pix_obj_cont(64);
    {
        HDF5Source<CartesianRecord<float>> source(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false);
        PixelizedObjectContainerSink sink(pix_obj_cont);
        FilterStream fs(&source, &sink, 10000, false);
        fs.run();
    }

    int lmax = 10;
    int nmax = 10;

    // Compare with Python
    auto python_cln = read_python_cln(lmax,nmax);

    KClkk kclkk = _decomp_SFB_FFT(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, false);
    kclkk.savetxt("sfb_reverse_fft");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                                    << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }

    kclkk = _decomp_SFB_FFT(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, false, true);
    kclkk.savetxt("sfb_reverse_fft_inter");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                            << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }
}

TEST(SFB, ReverseFFTParallel) {
    PixelizedObjectContainer pix_obj_cont(64);
    {
        HDF5Source<CartesianRecord<float>> source(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false);
        PixelizedObjectContainerSink sink(pix_obj_cont);
        FilterStream fs(&source, &sink, 10000, false);
        fs.run();
    }

    int lmax = 10;
    int nmax = 10;

    // Compare with Python
    auto python_cln = read_python_cln(lmax,nmax);

    KClkk kclkk = _decomp_SFB_FFT(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, true, false);
    kclkk.savetxt("sfb_reverse_fft_p");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                            << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }

    kclkk = _decomp_SFB_FFT(pix_obj_cont, lmax, nmax, 2500, 5535184878.03, false, true, true);
    kclkk.savetxt("sfb_reverse_fft_p_inter");

    for(int l=0; l<lmax; ++l){
        for(int n=0; n<nmax; ++n){
            EXPECT_NEAR(kclkk.c_ln(l,n), python_cln(l,n), 0.10*python_cln(l,n))
                            << "Error larger than 10% at l="<<l<<", n="<<n<<std::endl;
        }
    }
}