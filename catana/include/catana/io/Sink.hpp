#ifndef CATANA_SINK_HPP
#define CATANA_SINK_HPP

#include "../points/PointContainer.hpp"


namespace catana { namespace io {

  //! Parent Class Sink to write data of type Point class.
  /*!
   * Each child class must implement the two write functions. These must return the number of points which were written
   * and -1 if the writing failed.
   */
  class Sink {
  public:
    //! write points within [read_iterator, read_iterator+n) to sink.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of points
     * @param n number of points to read from read_iterator and write to sink
     * @return number of points written. -1 if failed
     */
    virtual long long int write(PointContainer::const_iterator read_iterator, size_t n) = 0;

    //! write points within [read_iterator, read_iterator+n) to sink.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of points
     * @param n number of points to read from read_iterator and write to sink
     * @return number of points written. -1 if failed
     */
    virtual long long int write(Point *read_iterator, size_t n) = 0;

    virtual ~Sink() = default;

    //! write entire point_container directly to sink
    /*!
     * @param point_container source container to write
     * @return number of points written. -1 if failed
     */
    long long int write(const PointContainer& point_container);
  };

}}

#endif //CATANA_SINK_HPP
