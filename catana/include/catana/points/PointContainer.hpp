#ifndef CATANA_POINTCONTAINER_HPP
#define CATANA_POINTCONTAINER_HPP

#include "Point.hpp"
#include <vector>


namespace catana {

  //! Container of Point: std::vector<Point>
  class PointContainer : public std::vector<Point> {
    // Inheriting constructors
    using std::vector<Point>::vector;
  public:
    //! Add an point to PointContainer
    void add_point(const Point& point) {
      push_back(point);
    }
  };

  //! Creates a random subset of the Point Container such that N points remain.
  /*!
   * This is a Fisher-Yates shuffle (stopped after N iterations).
   * See: https://en.wikipedia.org/wiki/Fisher–Yates_shuffle
   */
  void create_point_container_subset(PointContainer& point_container, const size_t& n);
}
#endif //CATANA_POINTCONTAINER_HPP
