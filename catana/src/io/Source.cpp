#include <catana/io/Source.hpp>
#include <catana/io/FilterStream.hpp>
#include <catana/io/sinks/PointContainerSink.hpp>
#include <catana/io/sinks/PixelizedPointContainerSink.hpp>


namespace catana { namespace io {

  size_t Source::get_npoints() {
    return 0;
  }

  PointContainer Source::get_point_container() {
    this->reset();
    PointContainer oc;
    PointContainerSink sink(oc);
    FilterStream fs(this, &sink);
    fs.run();
    return oc;
  }

  PixelizedPointContainer Source::get_pixelized_point_container(unsigned int nside) {
    this->reset();
    PixelizedPointContainer pix_oc(nside);
    PixelizedPointContainerSink sink(pix_oc);
    FilterStream fs(this, &sink);
    fs.run();
    return pix_oc;
  }

}}