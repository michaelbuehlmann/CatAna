#ifndef CATAN2_RANDOM_HPP
#define CATAN2_RANDOM_HPP

namespace catana {
  //! Initialize the random number generator with a random seed
  /*!
   * uses std::random_device to generate a random seed
   */
  void init_random();

  //! Initialize the random number generator with a specific seed
  /*!
   * generates reproducable results for repeated program runs
   *
   * @param seed the seed
   */
  void init_random(unsigned int seed);

}

#endif //CATAN2_RANDOM_HPP
