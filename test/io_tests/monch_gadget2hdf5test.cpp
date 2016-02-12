//
// Created by Michael Bühlmann on 12/02/16.
//

#include <catana/iotools.hpp>
#include <random>

#ifndef ALL_TESTS
std::mt19937 rng;


int main() {
    GadgetSource source("/mnt/lnec/buehmich/lpicola/ix1/i0/ix1_z0p000.0", true);
    HDF5Sink<SphericalRecord<float>> sink("/users/buehmich/MT/data/sdss_lpicola/ix1/t0/catalog_i0_test.hdf",
                                            "test", 1, 0, true, true);

    FilterStream fs(&source, &sink, 1000000, true);
    TophatRadialWindowFunctionFilter<float> tophat(2500/0.7);
    GaussianRadialWindowFunctionFilter<float> gauss(1000/0.7);

    fs.add_filter(&tophat);
    fs.add_filter(&gauss);

    fs.run();
    return 0;
}

#endif //ALL_TESTS