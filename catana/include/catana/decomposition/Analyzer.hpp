//
// Created by Michael Bühlmann on 18/02/16.
//

#ifndef CATANA_ANALYZER_HPP
#define CATANA_ANALYZER_HPP

#include "../iotools.hpp"
#include "../types.hpp"

#include <memory>
#include <vector>

class Analyzer {
public:
    Analyzer() = default;
    Analyzer(Source* source, double window_volume);

    void set_source(Source* source);
    void add_filter(Filter* filter);

    KClkk compute_sfb(unsigned short lmax, unsigned short nmax, double rmax, bool verbose);
    KClkk compute_sfb_pixelized(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside, bool verbose);
private:
    Source* source;
    std::vector<Filter*> filters;
    double window_volume;
};

#endif //CATANA_ANALYZER_HPP
