//
// Created by Michael Bühlmann on 08/02/16.
//

#ifndef CATANA_APP_FILTERS_HPP
#define CATANA_APP_FILTERS_HPP

#include "catana/tools/object_types.hpp"
#include <functional>
#include <cmath>
#include <random>
#include <healpix_cxx/healpix_map.h>
#include <healpix_cxx/healpix_map_fitsio.h>

extern std::mt19937 rng;

//! Base filter class.
/*
 * Child classes need to implement bool filter(Object&), which returns true if
 * object passes filtering and false if it needs to be removed.
 */
class Filter {
public:
    virtual bool filter(Object& object) = 0;
    virtual void operator()(ObjectContainer& object_container){
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
        object_container.resize(static_cast<size_t>(std::distance(object_container.begin(), end)));
    }
};

//! A generic radial filter. Window functionreturns the probability of object beeing accepted.
template<class FLOAT_TYPE>
class GenericRadialWindowFunctionFilter : public Filter{
public:
    GenericRadialWindowFunctionFilter(std::function<FLOAT_TYPE(FLOAT_TYPE)> window_function)
            : window_function(window_function), random_dist(0, 1) {}
    bool filter(Object& object) override {
        return (window_function(object.r) > random_dist(rng));
    }
private:
    std::function<FLOAT_TYPE(FLOAT_TYPE)> window_function;
    std::uniform_real_distribution<FLOAT_TYPE> random_dist;
};

//! A Gaussian window function with scale R0.
template<class FLOAT_TYPE>
class GaussianRadialWindowFunctionFilter : public GenericRadialWindowFunctionFilter<FLOAT_TYPE> {
public:
    GaussianRadialWindowFunctionFilter(FLOAT_TYPE R0)
            : GenericRadialWindowFunctionFilter<FLOAT_TYPE>(
                [R0](FLOAT_TYPE r){return std::exp(-std::pow(r/R0, 2));}
            ) {}
};

//! A radial tophat window function, only keeps objects within R0
template<class FLOAT_TYPE>
class TophatRadialWindowFunctionFilter : public Filter{
public:
    TophatRadialWindowFunctionFilter(FLOAT_TYPE R0)
            :R0(R0) {}

    virtual bool filter(Object& object){
        return object.r < R0;
    }

private:
    FLOAT_TYPE R0;

};

//! Creates a random subset of the Object Container such that N objects remain.
/*
 * This is a Fisher-Yates shuffle (stopped after N iterations).
 * See: https://en.wikipedia.org/wiki/Fisher–Yates_shuffle
 */
class SubsetFilter : public Filter {
public:
    SubsetFilter(size_t N) : N(N) {}

    bool filter(Object& object) override {return true;}  // This function is not needed
    void operator()(ObjectContainer& object_container) override {
        std::uniform_int_distribution<size_t> dist;  // default: (0, max(numeric type)

        auto current = object_container.begin();
        size_t obj_remaining = object_container.size();
        size_t obj_toadd = N;

        while(obj_toadd--) {
            auto random_pick = current + (dist(rng)%obj_remaining);
            std::swap(*current, *random_pick);
            ++current;
            --obj_remaining;
        }

        object_container.resize(N);
    }
private:
    size_t N;
};

//! Applies an angular mask (float healpix map)
/*
 * MASK==1: keep object at this pixel
 * MASK==0: remove object at this pixel
 */
class AngularMaskFilter : public Filter{
public:
    AngularMaskFilter(std::string healpix_mask)
    {
        read_Healpix_map_from_fits(healpix_mask, map);
    }

    bool filter(Object& object) override {
        return (map[map.ang2pix(object.p)]==1.f);
    }
private:
    Healpix_Map<float> map;
};

#endif //CATANA_APP_FILTERS_HPP

