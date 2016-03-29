//
// Created by Michael Bühlmann on 24/02/16.
//

#ifndef CATANA_OBJECTCONTAINERSOURCE_HPP
#define CATANA_OBJECTCONTAINERSOURCE_HPP

#include "../Source.hpp"

namespace catana { namespace io {
    //! A wrapper around an ObjectContainer which lets it act as Source
    class ObjectContainerSource : public Source {
    public:
        //! Constructor from ObjectContainer
        ObjectContainerSource(const ObjectContainer& object_container);

        //! Read next n objects from object_container. Returns number of objects put into object_s. -1 if EOF.
        /*!
         * @param write_iterator iterator of ObjectContainer of Objects. [write_iterator, write_iterator + n]
         * must be a valid range.
         * @param n number of objects to read
         */
        long long int read(ObjectContainer::iterator write_iterator, size_t n) override;

        //! Read next n objects from object_container. Returns number of objects put into object_s. -1 if EOF.
        /*!
         * @param write_iterator pointer to a Object[] C-array of Objects. [write_iterator, write_iterator + n]
         * must be a valid range.
         * @param n number of objects to read
         */
        long long int read(Object* write_iterator, size_t n) override;

        //! Resets Source to initial state (everything can be read again)
        void reset() override;

        //! Total number of objects which can be read (i.e. size of object_container)
        size_t get_nobjects() override;

    private:
        template<class ObjectIterator>
        long long int read_template(ObjectIterator write_iterator, size_t n);

    private:
        const ObjectContainer& object_container;
        ObjectContainer::const_iterator current;
    };

}}
#endif //CATANA_OBJECTCONTAINERSOURCE_HPP
