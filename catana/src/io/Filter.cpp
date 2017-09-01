#include <catana/io/Filter.hpp>
#include <random>


namespace catana { namespace io {

  size_t Filter::operator()(PointContainer& point_container, bool resize) {
    // end always points behind last element
    auto end = point_container.end();
    // current always points at the element currently beeing evaluated
    auto current = point_container.begin();
    while(current != end) {
      if(filter(*current)) {  // point is accepted
        ++current;
      } else {  // point is rejected
        std::swap(*current, *(--end));
      }
    }
    size_t new_size = static_cast<size_t>(std::distance(point_container.begin(), end));
    if(resize)
      point_container.resize(new_size);
    return new_size;
  }

  size_t Filter::operator()(Point *begin, Point *end) {
    Point *current = begin;
    while(current != end) {
      if(filter(*current)) {  // point is accepted
        ++current;
      } else {  // point is rejected
        std::swap(*current, *(--end));
      }
    }
    return end - begin;
  }

}}