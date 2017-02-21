#include <catana/io/Source.hpp>
#include <catana/io/FilterStream.hpp>
#include <catana/io/sinks/ObjectContainerSink.hpp>
#include <catana/io/sinks/PixelizedObjectContainerSink.hpp>


namespace catana { namespace io {

  size_t Source::get_nobjects() {
    return 0;
  }

  ObjectContainer Source::get_objectcontainer() {
    this->reset();
    ObjectContainer oc;
    ObjectContainerSink sink(oc);
    FilterStream fs(this, &sink);
    fs.run();
    return oc;
  }

  PixelizedObjectContainer Source::get_pixobjectcontainer(unsigned int nside) {
    this->reset();
    PixelizedObjectContainer pix_oc(nside);
    PixelizedObjectContainerSink sink(pix_oc);
    FilterStream fs(this, &sink);
    fs.run();
    return pix_oc;
  }

}}