#include "catana/io/Sink.hpp"

namespace catana { namespace io {

  long long int Sink::write(const PointContainer& point_container) {
    return write(point_container.begin(), point_container.size());
  }

}}