//
// Created by Michael Bühlmann on 15/02/16.
//

#include <catana/io_tools/Filter.hpp>
#include <random>

extern std::mt19937 rng;

size_t Filter::operator()(ObjectContainer& object_container, bool resize){
    // end always points behind last element
    auto end = object_container.end();
    // current always points at the element currently beeing evaluated
    auto current = object_container.begin();
    while(current != end){
        if(filter(*current)){  // object is accepted
            ++current;
        } else {  // object is rejected
            std::swap(*current, *(--end));
        }
    }
    size_t new_size = static_cast<size_t>(std::distance(object_container.begin(), end));
    if(resize)
        object_container.resize(new_size);
    return new_size;
}

size_t Filter::operator()(Object* begin, Object* end){
    Object* current = begin;
    while(current != end){
        if(filter(*current)){  // object is accepted
            ++current;
        } else {  // object is rejected
            std::swap(*current, *(--end));
        }
    }
    return end-begin;
}
