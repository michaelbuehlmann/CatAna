#ifndef CATANA_POINTCONTAINERSOURCE_HPP
#define CATANA_POINTCONTAINERSOURCE_HPP

#include "../Source.hpp"


namespace catana { namespace io {
  //! A wrapper around an PointContainer which lets it act as Source
  class PointContainerSource : public Source {
  public:
    //! Constructor from PointContainer
    PointContainerSource(const PointContainer& point_container);

    //! Read next n points from point_container. Returns number of points put into point_s. -1 if EOF.
    /*!
     * @param write_iterator iterator of PointContainer of Points. [write_iterator, write_iterator + n]
     * must be a valid range.
     * @param n number of points to read
     */
    long long int read(PointContainer::iterator write_iterator, size_t n) override;

    //! Read next n points from point_container. Returns number of points put into point_s. -1 if EOF.
    /*!
     * @param write_iterator pointer to a Point[] C-array of Points. [write_iterator, write_iterator + n]
     * must be a valid range.
     * @param n number of points to read
     */
    long long int read(Point *write_iterator, size_t n) override;

    //! Resets Source to initial state (everything can be read again)
    void reset() override;

    //! Total number of points which can be read (i.e. size of point_container)
    size_t get_npoints() override;

  private:
    template<class PointIterator>
    long long int read_template(PointIterator write_iterator, size_t n);

  private:
    const PointContainer& point_container;
    PointContainer::const_iterator current;
  };

}}
#endif //CATANA_POINTCONTAINERSOURCE_HPP
