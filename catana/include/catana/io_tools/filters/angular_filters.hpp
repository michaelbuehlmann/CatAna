//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_ANGULAR_FILTERS_HPP
#define CATANA_ANGULAR_FILTERS_HPP

#include "../Filter.hpp"
#include <healpix_cxx/healpix_map.h>

//! Applies an angular mask (float healpix map)
/*
 * MASK==1: keep object at this pixel
 * MASK==0: remove object at this pixel
 */
class AngularMaskFilter : public Filter{
public:
    AngularMaskFilter(std::string healpix_mask);
    bool filter(Object& object) override;
private:
    Healpix_Map<float> map;
};
#endif //CATANA_ANGULAR_FILTERS_HPP
