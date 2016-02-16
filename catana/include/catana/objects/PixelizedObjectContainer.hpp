//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_PIXELIZEDOBJECTCONTAINER_HPP
#define CATANA_PIXELIZEDOBJECTCONTAINER_HPP

#include "Object.hpp"
#include "ObjectContainer.hpp"
#include <healpix_cxx/healpix_base.h>
#include <healpix_cxx/pointing.h>
#include <vector>

class PixelObjects : public std::vector<double>{
public:
    pointing p;
};


class PixelizedObjectContainer : public std::vector<PixelObjects> {
public:
//    PixelizedObjectContainer() = default;
    PixelizedObjectContainer(unsigned int nside);
    PixelizedObjectContainer(unsigned int nside, const ObjectContainer& oc);
    void add_object(const Object&);
    unsigned int get_nside() const;
    size_t get_nobjects() const;

//    using std::vector<PixelObjects>::operator[];
//    using std::vector<PixelObjects>::size;
//    using std::vector<PixelObjects>::begin;
//    using std::vector<PixelObjects>::end;

private:
    Healpix_Base hp_base;
    unsigned int nside;
};

#endif //CATANA_PIXELIZEDOBJECTCONTAINER_HPP
