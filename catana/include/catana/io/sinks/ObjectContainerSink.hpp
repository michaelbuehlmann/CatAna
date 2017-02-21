#ifndef CATANA_OBJECTCONTAINERSINK_HPP
#define CATANA_OBJECTCONTAINERSINK_HPP

namespace catana { namespace io {

  //! A wrapper around an ObjectContainer which lets it act as Sink
  class ObjectContainerSink : public Sink {
  public:
    //! Construct from ObjectContainer. object_container will be filled with data.
    ObjectContainerSink(ObjectContainer& object_container)
        : object_container(object_container) {};

    //! write objects within [read_iterator, read_iterator+n) to object_container.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
     * @param n number of objects to read from read_iterator and write to sink
     * @return number of objects written (=n)
     */
    long long int write(ObjectContainer::iterator read_iterator, size_t n) { return write_template(read_iterator, n); }

    //! write objects within [read_iterator, read_iterator+n) to object_container.
    /*!
     * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
     * @param n number of objects to read from read_iterator and write to sink
     * @return number of objects written (=n)
     */
    long long int write(Object *read_iterator, size_t n) { return write_template(read_iterator, n); }

  private:
    template<class ObjectIterator>
    long long int write_template(ObjectIterator read_iterator, size_t n) {
      for(size_t i = 0; i < n; ++i) {
        object_container.push_back(*(read_iterator++));
      }
      return n;
    }

  private:
    ObjectContainer& object_container;
  };

}}
#endif //CATANA_OBJECTCONTAINERSINK_HPP
