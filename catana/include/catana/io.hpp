#ifndef CATANA_APP_IO_HPP
#define CATANA_APP_IO_HPP

#include "io/record_types.hpp"
#include "io/Source.hpp"
#include "io/Sink.hpp"

#include "io/sources/ObjectContainerSource.hpp"
#include "io/sources/GadgetSource.hpp"
#include "io/sources/RawBinarySource.hpp"
#include "io/sources/TextSource.hpp"

#include "io/sinks/ObjectContainerSink.hpp"
#include "io/sinks/PixelizedObjectContainerSink.hpp"
#include "io/sinks/RawBinarySink.hpp"
#include "io/sinks/TextSink.hpp"

#include "io/Filter.hpp"
#include "io/filters/angular_filters.hpp"
#include "io/filters/radial_filters.hpp"

#include "io/FilterStream.hpp"


#endif //CATANA_APP_IO_HPP
