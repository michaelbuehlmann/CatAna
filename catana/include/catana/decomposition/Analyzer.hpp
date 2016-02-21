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
    Analyzer(std::shared_ptr<Source> source_ptr, double window_volume);

    void set_source(std::shared_ptr<Source> source);
    void add_filter(std::shared_ptr<Filter> filter);

    KClkk compute_sfb(unsigned short lmax, unsigned short nmax, double rmax, bool verbose);
    KClkk compute_sfb_pixelized(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside, bool verbose);
private:
    std::shared_ptr<Source> source;
    std::vector<std::shared_ptr<Filter>> filters;
    double window_volume;
};

#endif //CATANA_ANALYZER_HPP
