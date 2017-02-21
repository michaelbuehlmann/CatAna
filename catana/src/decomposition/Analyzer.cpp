#include <catana/decomposition/Analyzer.hpp>
#include <catana/decomposition/sfb_decomposition.hpp>
#include <unistd.h>


namespace catana {
  Analyzer::Analyzer(io::Source *source, double window_volume)
      : source(source), window_volume(window_volume) {}

  void Analyzer::set_source(io::Source *source) {
    this->source = source;
  }

  void Analyzer::set_subsample_size(unsigned int n) {
    subsample_size = n;
  }

  void Analyzer::add_filter(io::Filter *filter) {
    filters.push_back(filter);
  }

  KClkk Analyzer::compute_sfb(unsigned short lmax, unsigned short nmax, double rmax,
                              bool store_flmn, bool verbose) {
    // TODO: Location of temporary file
    std::string temp_filename = std::string("tmp_") + std::to_string(::getpid());
    source->reset();
    ObjectContainer oc;
    {
      io::ObjectContainerSink sink(oc);
      io::FilterStream fs(source, &sink, 1000000, verbose);
      io::TophatRadialWindowFunctionFilter radial_filter(rmax);
      fs.add_filter(&radial_filter);
      for(auto& filter: filters) {
        fs.add_filter(filter);
      }
      fs.run(subsample_size, temp_filename);
    }
    return sfb_decomposition(oc, lmax, nmax, rmax, window_volume, store_flmn, verbose);
  }

  KClkk Analyzer::compute_sfb_pixelized(unsigned short lmax, unsigned short nmax, double rmax, unsigned int nside,
                                        bool store_flmn, bool verbose) {
    std::string temp_filename = std::string("tmp_") + std::to_string(::getpid());
    source->reset();
    PixelizedObjectContainer pix_oc(nside);

    {
      io::PixelizedObjectContainerSink sink(pix_oc);
      io::FilterStream fs(source, &sink, 1000000, verbose);
      io::TophatRadialWindowFunctionFilter radial_filter(rmax);
      fs.add_filter(&radial_filter);
      for(auto& filter: filters) {
        fs.add_filter(filter);
      }
      fs.run(subsample_size, temp_filename);
    }
    return sfb_decomposition(pix_oc, lmax, nmax, rmax, window_volume, store_flmn, verbose);
  }

}