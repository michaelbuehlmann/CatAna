#ifndef CATANA_SOURCE_HPP
#define CATANA_SOURCE_HPP

#include "../points/Point.hpp"
#include "../points/PointContainer.hpp"
#include "../points/PixelizedPointContainer.hpp"
#include <algorithm>


namespace catana { namespace io {

  //! Parent Class Source to read data of type Point class.
  /*!
   * Each child class must implement the two read functions. These must return the number of points which were read
   * (i.e. put into the point array) and -1 if no more Points are available.
   */
  class Source {
  public:
    //! Number of points which can be read. Not needed nessessarily.
    virtual size_t get_npoints();

    //! Read next n points. Returns number of points put into write_iterator. -1 if EOF.
    virtual long long int read(PointContainer::iterator write_iterator, size_t n) = 0;

    //! Read next n points. Returns number of points put into write_iterator. -1 if EOF.
    virtual long long int read(Point *write_iterator, size_t n) = 0;

    //! Reset the source, i.e. revert to initial state.
    virtual void reset() = 0;

    virtual ~Source() = default;

  public:
    //! Directly load all data into an PointContainer (no FilterStream needed).
    PointContainer get_point_container();

    //! Directly load all data into a PixelizedPointContainer with given NSide (no FilterStream needed).
    PixelizedPointContainer get_pixelized_point_container(unsigned int nside);
  };

}}
#endif //CATANA_SOURCE_HPP
