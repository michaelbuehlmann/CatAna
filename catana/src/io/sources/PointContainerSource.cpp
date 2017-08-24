#include <catana/io/sources/PointContainerSource.hpp>


namespace catana { namespace io {

  PointContainerSource::PointContainerSource(const PointContainer& point_container)
      : point_container(point_container), current(point_container.begin()) {};

  long long int PointContainerSource::read(PointContainer::iterator write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  long long int PointContainerSource::read(Point *write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  void PointContainerSource::reset() {
    current = point_container.begin();
  }

  size_t PointContainerSource::get_npoints() {
    return point_container.size();
  }

  template<class PointIterator>
  long long int PointContainerSource::read_template(PointIterator write_iterator, size_t n) {
    size_t to_read = std::min(n, static_cast<size_t>(std::distance(current, point_container.end())));

    // Abort if nothing to read
    if(to_read == 0) {
      return -1;
    }

    for(size_t i = 0; i < to_read; ++i) {
      *(write_iterator++) = *(current++);
    }
    return to_read;
  }

}}