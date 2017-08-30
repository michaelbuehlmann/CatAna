#ifndef CATANA_POINTCONTAINERSINK_HPP
#define CATANA_POINTCONTAINERSINK_HPP


#include <catana/io/Sink.hpp>


namespace catana { namespace io {

  //! A wrapper around an PointContainer which lets it act as Sink
  class PointContainerSink : public Sink {
  public:
    //! Construct from PointContainer. point_container will be filled with data.
    PointContainerSink(PointContainer& point_container)
        : point_container(point_container) {};

    //! write points within [read_iterator, read_iterator+n) to point_container.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of points
     * @param n number of points to read from read_iterator and write to sink
     * @return number of points written (=n)
     */
    long long int write(PointContainer::const_iterator read_iterator, size_t n) { return write_template(read_iterator, n); }

    //! write points within [read_iterator, read_iterator+n) to point_container.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of points
     * @param n number of points to read from read_iterator and write to sink
     * @return number of points written (=n)
     */
    long long int write(Point *read_iterator, size_t n) { return write_template(read_iterator, n); }

    using Sink::write;

  private:
    template<class PointIterator>
    long long int write_template(PointIterator read_iterator, size_t n) {
      for(size_t i = 0; i < n; ++i) {
        point_container.push_back(*(read_iterator++));
      }
      return n;
    }

  private:
    PointContainer& point_container;
  };

}}
#endif //CATANA_POINTCONTAINERSINK_HPP
