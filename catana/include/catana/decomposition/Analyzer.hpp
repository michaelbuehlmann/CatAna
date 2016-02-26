//
// Created by Michael Bühlmann on 18/02/16.
//

#ifndef CATANA_ANALYZER_HPP
#define CATANA_ANALYZER_HPP

#include "../iotools.hpp"
#include "../types.hpp"

#include <memory>
#include <vector>

namespace catana {
    class Analyzer {
    public:
        Analyzer() = default;

        Analyzer(io::Source* source, double window_volume);

        void set_source(io::Source* source);

        void add_filter(io::Filter* filter);

        KClkk compute_sfb(unsigned short lmax, unsigned short nmax, double rmax, bool verbose);

        KClkk compute_sfb_pixelized(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside,
                bool verbose);

    private:
        io::Source* source;
        std::vector<io::Filter*> filters;
        double window_volume;
    };

}
#endif //CATANA_ANALYZER_HPP
