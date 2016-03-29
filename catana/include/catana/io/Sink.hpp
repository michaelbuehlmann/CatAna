//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_SINK_HPP
#define CATANA_SINK_HPP

#include "../types.hpp"

namespace catana { namespace io {

    //! Parent Class Sink to write data of type Object class.
    /*!
     * Each child class must implement the two write functions. These must return the number of objects which were written
     * and -1 if the writing failed.
     */
    class Sink {
    public:
        //! write objects within [read_iterator, read_iterator+n) to sink.
        /*!
         * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
         * @param n number of objects to read from read_iterator and write to sink
         * @return number of objects written. -1 if failed
         */
        virtual long long int write(ObjectContainer::iterator read_iterator, size_t n) = 0;

        //! write objects within [read_iterator, read_iterator+n) to sink.
        /*!
         * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
         * @param n number of objects to read from read_iterator and write to sink
         * @return number of objects written. -1 if failed
         */
        virtual long long int write(Object* read_iterator, size_t n) = 0;

        virtual ~Sink()=default;
    };

}}

#endif //CATANA_SINK_HPP
