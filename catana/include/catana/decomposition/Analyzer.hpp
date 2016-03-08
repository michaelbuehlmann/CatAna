//
// Created by Michael Bühlmann on 18/02/16.
//

#ifndef CATANA_ANALYZER_HPP
#define CATANA_ANALYZER_HPP

#include "../iotools.hpp"
#include "../types.hpp"
#include "return_types.hpp"

#include <memory>
#include <vector>

namespace catana {
    class Analyzer {
    public:
        Analyzer() = default;

        Analyzer(io::Source* source, double window_volume);

        void set_source(io::Source* source);
        void set_subsample_size(unsigned int n);

        void add_filter(io::Filter* filter);

        KClkk compute_sfb(unsigned short lmax, unsigned short nmax, double rmax,
                bool store_flmn, bool verbose);

        KClkk compute_sfb_pixelized(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside,
                bool store_flmn, bool verbose);

    private:
        io::Source* source;
        std::vector<io::Filter*> filters;
        double window_volume;
        unsigned int subsample_size = 0;
    };

}
#endif //CATANA_ANALYZER_HPP
