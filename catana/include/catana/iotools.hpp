//
// Created by Michael Bühlmann on 05/02/16.
//

#ifndef CATANA_APP_IO_TOOLS_HPP
#define CATANA_APP_IO_TOOLS_HPP

#include "io_tools/record_types.hpp"
#include "io_tools/Source.hpp"
#include "io_tools/Sink.hpp"

#include "io_tools/sources/ObjectContainerSource.hpp"
#include "io_tools/sources/HDF5Source.hpp"
#include "io_tools/sources/GadgetSource.hpp"
#include "io_tools/sources/RawBinarySource.hpp"
#include "io_tools/sources/TextSource.hpp"

#include "io_tools/sinks/ObjectContainerSink.hpp"
#include "io_tools/sinks/PixelizedObjectContainerSink.hpp"
#include "io_tools/sinks/HDF5Sink.hpp"
#include "io_tools/sinks/RawBinarySink.hpp"
#include "io_tools/sinks/TextSink.hpp"

#include "io_tools/Filter.hpp"
#include "io_tools/filters/angular_filters.hpp"
#include "io_tools/filters/radial_filters.hpp"

#include "io_tools/FilterStream.hpp"

#endif //CATANA_APP_IO_TOOLS_HPP
