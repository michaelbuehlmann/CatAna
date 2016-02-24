//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_SOURCE_HPP
#define CATANA_SOURCE_HPP

#include "../types.hpp"
#include <algorithm>

//! Parent Class Source to read data of type "Object".
/*
 * Each child class must implement the two read functions. These must return the number of objects which were read
 * (i.e. put into the object array) and -1 if no more Objects are available.
 */
class Source {
public:
    // These Functions need to be overridden
    virtual size_t get_nobjects();
    virtual long long int read(ObjectContainer::iterator write_iterator, size_t n) = 0;
    virtual long long int read(Object* write_iterator, size_t n) = 0;
    virtual void reset() = 0;

public:
    ObjectContainer get_objectcontainer();
    PixelizedObjectContainer get_pixobjectcontainer(unsigned int nside);
};

#endif //CATANA_SOURCE_HPP
