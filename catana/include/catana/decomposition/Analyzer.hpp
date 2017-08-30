#ifndef CATANA_ANALYZER_HPP
#define CATANA_ANALYZER_HPP

#include "../io/Source.hpp"
#include "../io/Filter.hpp"
#include "return_types.hpp"

#include <memory>
#include <vector>


namespace catana {
  //! The Analyzer Class combines input (source, filters) and decomposition.
  class Analyzer {
  public:
    Analyzer() = default;

    //! Construct from a child class of source and a window volume for normalization
    Analyzer(io::Source *source);

    //! Reset source
    void set_source(io::Source *source);

    //! If only a random subset of source shall be decomposed, set subsample size here
    void set_subsample_size(unsigned int n);

    //! If the input source shall be filtered, add filter instances
    void add_filter(io::Filter *filter);

    //! Do the decomposition with the raw (brute-force) method
    KClkk compute_sfb(unsigned short lmax, unsigned short nmax, double rmax,
                      bool store_flmn, bool verbose);

    //! Do the decomposition with the pixelized method using HEALPix and FFTW
    KClkk compute_sfb_pixelized(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside,
                                bool store_flmn, bool verbose);

  private:
    io::Source *source;
    std::vector<io::Filter *> filters;
    unsigned int subsample_size = 0;
  };

}
#endif //CATANA_ANALYZER_HPP
