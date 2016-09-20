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
#include <Eigen/Dense>

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

    Eigen::ArrayXi get_countmap() const;

    const Healpix_Base& get_hp_base() const;

private:
    Healpix_Base hp_base;
    unsigned int nside;
};

#endif //CATANA_PIXELIZEDOBJECTCONTAINER_HPP
