//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_SOURCE_HPP
#define CATANA_SOURCE_HPP

#include <catana/types.hpp>
#include <algorithm>

//! Parent Class Source to read data of type "Object".
/*
 * Each child class must implement the two read functions. These must return the number of objects which were read
 * (i.e. put into the object array) and -1 if no more Objects are available.
 */
class Source {
public:
    virtual size_t get_nobjects() {return 0;}
    virtual long long int read(ObjectContainer::iterator write_iterator, size_t n) = 0;
    virtual long long int read(Object* write_iterator, size_t n) = 0;
};

class ObjectContainerSource : public Source {
public:
    ObjectContainerSource(const ObjectContainer& object_container)
            : object_container(object_container), current(object_container.begin())
    {};

    long long int read(ObjectContainer::iterator write_iterator, size_t n) {return read_template(write_iterator, n);};
    long long int read(Object* write_iterator, size_t n) {return read_template(write_iterator, n);};

private:
    template<class ObjectIterator>
    long long int read_template(ObjectIterator write_iterator, size_t n) {
        size_t to_read = std::min(n, static_cast<size_t>(std::distance(current, object_container.end())));

        // Abort if nothing to read
        if(to_read==0){
            return -1;
        }

        for(size_t i=0; i<to_read; ++i) {
            *(write_iterator++) = *(current++);
        }
        return to_read;
    }
private:
    const ObjectContainer& object_container;
    ObjectContainer::const_iterator current;
};

#endif //CATANA_SOURCE_HPP
