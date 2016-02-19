//
// Created by Michael Bühlmann on 18/02/16.
//

#include <catana/decomposition/Analyzer.hpp>
#include <catana/decomposition/sfb_decomposition.hpp>

Analyzer::Analyzer(std::shared_ptr<Source> source, double window_volume)
    : source(source), window_volume(window_volume)
{}

void Analyzer::set_source(std::shared_ptr<Source> source)
{
    this->source = source;
}

void Analyzer::add_filter(std::shared_ptr<Filter> filter)
{
    filters.push_back(filter);
}

KClkk Analyzer::compute_sfb(unsigned short lmax, unsigned short nmax, double rmax, bool verbose)
{
    ObjectContainer oc;
    {
        ObjectContainerSink sink(oc);
        FilterStream fs(source.get(), &sink);
        for (auto& filter: filters) {
            fs.add_filter(filter.get());
        }
        fs.run();
    }
    return _decomp_SFB(oc, lmax, nmax, rmax, window_volume, verbose, true);
}

KClkk Analyzer::compute_sfb(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside, bool verbose)
{
    PixelizedObjectContainer pix_oc(nside);
    {
        PixelizedObjectContainerSink sink(pix_oc);
        FilterStream fs(source.get(), &sink);
        for (auto& filter: filters) {
            fs.add_filter(filter.get());
        }
        fs.run();
    }
    return _decomp_SFB(pix_oc, lmax, nmax, rmax, window_volume, verbose, true);
}
