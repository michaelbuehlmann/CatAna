//
// Created by Michael Bühlmann on 21/02/16.
//

#include <catana/io.hpp>
#include <catana/decomposition/Analyzer.hpp>
#include <gtest/gtest.h>

// LOAD TEST_DATA_DIR
#include <catana/config.hpp>
const std::string test_data_dir(TEST_DATA_DIR);

#ifndef ALL_TESTS
#include <random>
std::mt19937 rng;
#endif //ALL_TESTS

using namespace catana;

TEST(Analyzer, RAW){
    auto source = std::shared_ptr<io::Source>(new io::TextSource<io::CartesianRecord<float>>(test_data_dir+"gaussian_catalog.txt", 1, 0));
//    auto source = std::shared_ptr<io::Source>(new io::HDF5Source<io::CartesianRecord<float>>(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false));
    Analyzer analyzer(
            source.get(),
            5535184878.03
    );

    auto kclkk = analyzer.compute_sfb(5, 10, 2500, false, false);
}

TEST(Analyzer, Pixelized){
    auto source = std::shared_ptr<io::Source>(new io::TextSource<io::CartesianRecord<float>>(test_data_dir+"gaussian_catalog.txt", 1, 0));
//    auto source = std::shared_ptr<io::Source>(new io::HDF5Source<io::CartesianRecord<float>>(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false));
    Analyzer analyzer(
            source.get(),
            5535184878.03
    );

    auto kclkk = analyzer.compute_sfb_pixelized(5, 10, 2500, 16, false, false);
}

TEST(Analyzer, Filter){
    auto source = std::shared_ptr<io::Source>(new io::TextSource<io::CartesianRecord<float>>(test_data_dir+"gaussian_catalog.txt", 1, 0));
//    auto source = std::shared_ptr<io::Source>(new io::HDF5Source<io::CartesianRecord<float>>(test_data_dir+"gaussian_catalog.hdf", "particle_pos_cartesian", 1, 0, false));
    Analyzer analyzer(
            source.get(),
            5535184878.03
    );
    auto filter = std::shared_ptr<io::Filter>(new io::TophatRadialWindowFunctionFilter(1000));
    analyzer.add_filter(
            filter.get()
    );
    auto kclkk = analyzer.compute_sfb_pixelized(5, 10, 2500, 16, false, false);
}