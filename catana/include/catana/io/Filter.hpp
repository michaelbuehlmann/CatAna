#ifndef CATANA_APP_FILTERS_HPP
#define CATANA_APP_FILTERS_HPP

#include "../points/Point.hpp"
#include "../points/PointContainer.hpp"


namespace catana { namespace io {

  //! Parent class Filter for filtering data of type Point.
  /*!
   * Child classes need to implement bool filter(Point&), which returns true if
   * point passes filtering and false if it needs to be removed.
   */
  class Filter {
  public:
    //! Filtering function on Point. Returns true if point passes filter, false otherwise.
    virtual bool filter(Point& point) = 0;

    //! Run filter on an point_container
    /*!
     * @param point_container point_container on which to apply filter
     * @param resize if true, resize the point container. if false, the filtered data will be within
     * [point_container.begin(), point_container.begin()+n), where n is the return value
     * @return number of points remaining after filtering (== point_container.size() if resize==true)
     */
    size_t operator()(PointContainer& point_container, bool resize = true);

    //! Run filter on a C-array of Points
    /*!
     * Filter points between [begin, end). Remaining points will be stored between [begin, begin +n), where
     * n is the return value
     * @return number of points remaining after filtering.
     */
    virtual size_t operator()(Point *begin, Point *end);
  };

}}

#endif //CATANA_APP_FILTERS_HPP

