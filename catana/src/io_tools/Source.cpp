//
// Created by Michael Bühlmann on 24/02/16.
//

#include <catana/io_tools/Source.hpp>
#include <catana/io_tools/FilterStream.hpp>
#include <catana/io_tools/sinks/ObjectContainerSink.hpp>
#include <catana/io_tools/sinks/PixelizedObjectContainerSink.hpp>

namespace catana {namespace io {

        size_t Source::get_nobjects()
        {
            return 0;
        }

        ObjectContainer Source::get_objectcontainer()
        {
            this->reset();
            ObjectContainer oc;
            ObjectContainerSink sink(oc);
            FilterStream fs(this, &sink);
            fs.run();
            return oc;
        }

        PixelizedObjectContainer Source::get_pixobjectcontainer(unsigned int nside)
        {
            this->reset();
            PixelizedObjectContainer pix_oc(nside);
            PixelizedObjectContainerSink sink(pix_oc);
            FilterStream fs(this, &sink);
            fs.run();
            return pix_oc;
        }

}}