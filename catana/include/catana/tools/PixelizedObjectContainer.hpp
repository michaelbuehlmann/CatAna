//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_PIXELIZEDOBJECTCONTAINER_HPP
#define CATANA_PIXELIZEDOBJECTCONTAINER_HPP

#include "../tools/Object.hpp"
#include <healpix_cxx/healpix_base.h>
#include <healpix_cxx/pointing.h>
#include <vector>

class PixelObjects : public std::vector<double>{
public:
    pointing p;
};


class PixelizedObjectContainer : private std::vector<PixelObjects> {
public:
    PixelizedObjectContainer() = default;
    PixelizedObjectContainer(unsigned int nside);
    void add_object(const Object&);
    unsigned int get_nside();
    size_t get_nobjects();

    using std::vector<PixelObjects>::operator[];
    using std::vector<PixelObjects>::size;
    using std::vector<PixelObjects>::begin;
    using std::vector<PixelObjects>::end;

private:
    Healpix_Base hp_base;
    unsigned int nside;
};

#endif //CATANA_PIXELIZEDOBJECTCONTAINER_HPP
