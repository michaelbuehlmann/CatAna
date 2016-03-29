//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_PIXELIZEDOBJECTCONTAINERSINK_HPP
#define CATANA_PIXELIZEDOBJECTCONTAINERSINK_HPP

#include "../Sink.hpp"

namespace catana { namespace io {

    //! A wrapper around an PixelizedObjectContainer which lets it act as Sink
    class PixelizedObjectContainerSink : public Sink {
    public:
        //! Construct from PixelizedObjectContainer. pixelized_object_container will be filled with data.
        PixelizedObjectContainerSink(PixelizedObjectContainer& pixelized_object_container)
                :pixelized_object_container(pixelized_object_container) { };

        //! write objects within [read_iterator, read_iterator+n) to pixelized_object_container.
        /*!
         * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
         * @param n number of objects to read from read_iterator and write to sink
         * @return number of objects written (=n)
         */
        long long int write(ObjectContainer::iterator read_iterator, size_t n)
        {
            return write_template(read_iterator, n);
        };

        //! write objects within [read_iterator, read_iterator+n) to pixelized_object_container.
        /*!
         * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
         * @param n number of objects to read from read_iterator and write to sink
         * @return number of objects written (=n)
         */
        long long int write(Object* read_iterator, size_t n) { return write_template(read_iterator, n); };
    private:
        template<class ObjectIterator>
        long long int write_template(ObjectIterator read_iterator, size_t n)
        {
            for (size_t i = 0; i<n; ++i) {
                pixelized_object_container.add_object(*(read_iterator++));
            }
            return n;
        }

    private:
        PixelizedObjectContainer& pixelized_object_container;
    };

}}
#endif //CATANA_PIXELIZEDOBJECTCONTAINERSINK_HPP
