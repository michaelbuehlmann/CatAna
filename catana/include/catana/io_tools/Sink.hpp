//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_SINK_HPP
#define CATANA_SINK_HPP

#include "../types.hpp"

class Sink {
public:
    virtual long long int write(ObjectContainer::iterator read_iterator, size_t n) = 0;
    virtual long long int write(Object* read_iterator, size_t n) = 0;
};

#endif //CATANA_SINK_HPP
