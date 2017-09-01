#include "doctest.h"
#include <catana/catana.hpp>

// LOAD TEST_DATA_DIR
#include <catana/config.hpp>


const std::string test_data_dir(TEST_DATA_DIR);

using namespace catana;

TEST_CASE ("running Analyzer with raw method") {
  auto source = std::shared_ptr<io::Source>(
      new io::TextSource<io::CartesianRecord<float>>(test_data_dir + "gaussian_catalog.txt", 0));
  Analyzer analyzer(source.get());

  auto kclkk = analyzer.compute_sfb(5, 10, 2500, false, false);
}

TEST_CASE ("running analyzer with pixel method") {
  auto source = std::shared_ptr<io::Source>(
      new io::TextSource<io::CartesianRecord<float>>(test_data_dir + "gaussian_catalog.txt", 0));
  Analyzer analyzer(source.get());

  auto kclkk = analyzer.compute_sfb_pixelized(5, 10, 2500, 16, false, false);
}

TEST_CASE ("running Analyzer with pixel method and filtering") {
  auto source = std::shared_ptr<io::Source>(
      new io::TextSource<io::CartesianRecord<float>>(test_data_dir + "gaussian_catalog.txt", 0));
  Analyzer analyzer(source.get());
  auto filter = std::shared_ptr<io::Filter>(new io::TophatRadialWindowFunctionFilter(1000));
  analyzer.add_filter(
      filter.get()
  );
  auto kclkk = analyzer.compute_sfb_pixelized(5, 10, 2500, 16, false, false);
}