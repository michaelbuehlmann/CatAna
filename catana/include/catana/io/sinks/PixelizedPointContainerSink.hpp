#ifndef CATANA_PIXELIZEDPOINTCONTAINERSINK_HPP
#define CATANA_PIXELIZEDPOINTCONTAINERSINK_HPP

#include "../Sink.hpp"


namespace catana { namespace io {

  //! A wrapper around an PixelizedPointContainer which lets it act as Sink
  class PixelizedPointContainerSink : public Sink {
  public:
    //! Construct from PixelizedPointContainer. pixelized_point_container will be filled with data.
    PixelizedPointContainerSink(PixelizedPointContainer& pixelized_point_container)
        : pixelized_point_container(pixelized_point_container) {};

    //! write points within [read_iterator, read_iterator+n) to pixelized_point_container.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of points
     * @param n number of points to read from read_iterator and write to sink
     * @return number of points written (=n)
     */
    long long int write(PointContainer::const_iterator read_iterator, size_t n) {
      return write_template(read_iterator, n);
    };

    //! write points within [read_iterator, read_iterator+n) to pixelized_point_container.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of points
     * @param n number of points to read from read_iterator and write to sink
     * @return number of points written (=n)
     */
    long long int write(Point *read_iterator, size_t n) { return write_template(read_iterator, n); };

    using Sink::write;

  private:
    template<class PointIterator>
    long long int write_template(PointIterator read_iterator, size_t n) {
      for(size_t i = 0; i < n; ++i) {
        pixelized_point_container.add_point(*(read_iterator++));
      }
      return n;
    }

  private:
    PixelizedPointContainer& pixelized_point_container;
  };

}}
#endif //CATANA_PIXELIZEDPOINTCONTAINERSINK_HPP
