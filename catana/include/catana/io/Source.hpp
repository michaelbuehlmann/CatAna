#ifndef CATANA_SOURCE_HPP
#define CATANA_SOURCE_HPP

#include "../types.hpp"
#include <algorithm>


namespace catana { namespace io {

  //! Parent Class Source to read data of type Object class.
  /*!
   * Each child class must implement the two read functions. These must return the number of objects which were read
   * (i.e. put into the object array) and -1 if no more Objects are available.
   */
  class Source {
  public:
    //! Number of objects which can be read. Not needed nessessarily.
    virtual size_t get_nobjects();

    //! Read next n objects. Returns number of objects put into write_iterator. -1 if EOF.
    virtual long long int read(ObjectContainer::iterator write_iterator, size_t n) = 0;

    //! Read next n objects. Returns number of objects put into write_iterator. -1 if EOF.
    virtual long long int read(Object *write_iterator, size_t n) = 0;

    //! Reset the source, i.e. revert to initial state.
    virtual void reset() = 0;

    virtual ~Source() = default;

  public:
    //! Directly load all data into an ObjectContainer (no FilterStream needed).
    ObjectContainer get_objectcontainer();

    //! Directly load all data into a PixelizedObjectContainer with given NSide (no FilterStream needed).
    PixelizedObjectContainer get_pixobjectcontainer(unsigned int nside);
  };

}}
#endif //CATANA_SOURCE_HPP
