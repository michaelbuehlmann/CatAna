#include <random>
#include <array>
#include <algorithm>
#include <functional>
#include "catana/tools/random.hpp"


namespace catana {
  std::random_device rand_dev;
  std::mt19937 rng;

  void init_random(unsigned int seed) {
    rng.seed(seed);
  }

  void init_random() {
    std::array<int, 624> seed_data;
    std::generate(seed_data.begin(), seed_data.end(), std::ref(rand_dev));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    rng.seed(seq);
  }
}