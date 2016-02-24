//
// Created by Michael Bühlmann on 18/02/16.
//

#include <catana/decomposition/Analyzer.hpp>
#include <catana/decomposition/sfb_decomposition.hpp>

Analyzer::Analyzer(Source* source, double window_volume)
    : source(source), window_volume(window_volume)
{}

void Analyzer::set_source(Source* source)
{
    this->source = source;
}

void Analyzer::add_filter(Filter* filter)
{
    filters.push_back(filter);
}

KClkk Analyzer::compute_sfb(unsigned short lmax, unsigned short nmax, double rmax, bool verbose)
{
    source->reset();
    ObjectContainer oc;
    {
        ObjectContainerSink sink(oc);
        FilterStream fs(source, &sink, 1000000, verbose);
        TophatRadialWindowFunctionFilter radial_filter(rmax);
        fs.add_filter(&radial_filter);
        for (auto& filter: filters) {
            fs.add_filter(filter);
        }
        fs.run();
    }
    return sfb_decomposition(oc, lmax, nmax, rmax, window_volume, verbose);
}

KClkk Analyzer::compute_sfb_pixelized(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside,
        bool verbose)
{
    source->reset();
    PixelizedObjectContainer pix_oc(nside);
    {
        PixelizedObjectContainerSink sink(pix_oc);
        FilterStream fs(source, &sink, 1000000, verbose);
        TophatRadialWindowFunctionFilter radial_filter(rmax);
        fs.add_filter(&radial_filter);
        for (auto& filter: filters) {
            fs.add_filter(filter);
        }
        fs.run();
    }
    return sfb_decomposition(pix_oc, lmax, nmax, rmax, window_volume, verbose);
}
