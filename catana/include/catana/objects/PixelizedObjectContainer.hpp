//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_PIXELIZEDOBJECTCONTAINER_HPP
#define CATANA_PIXELIZEDOBJECTCONTAINER_HPP

#include "Object.hpp"
#include "ObjectContainer.hpp"
#include <healpix_base.h>
#include <pointing.h>
#include <vector>
#include <Eigen/Dense>

namespace catana {

    //! Pixel on spherical shell containing objects at different distances r: std::vector<double>
    class PixelObjects : public std::vector<double> {
    public:
        pointing p; //!< the angular position of the pixel: a HEALPix pointing
    };

    //! A HEALPix Map of PixelObjects
    /*!
     * DO NOT CHANGE SIZE (push_back, resize, etc) of underlying std::vector once constructed!
     */
    class PixelizedObjectContainer : public std::vector<PixelObjects> {
    public:
        //! Construct for given nside.
        /*!
         * The instance will have size nside2npix(nside) (see HEALPix documentation).
         * Each PixelObjects will have size 0.
         */
        PixelizedObjectContainer(unsigned int nside);

        //! Construct for given nside from an existing ObjectContainer.
        /*!
         * The instance will have size nside2npix(nside) (see HEALPix documentation)
         * Each Pixel is assigned the according Objects from ObjectContainer
         */
        PixelizedObjectContainer(unsigned int nside, const ObjectContainer& oc);

        //! Add object to PixelizedObjectContainer
        void add_object(const Object&);

        //! Get the NSide
        unsigned int get_nside() const;

        //! Get number of objects stored within PixelizedObjectContainer (i.e. sum objects over all pixels)
        size_t get_nobjects() const;

        //! Get a HEALPix map (RING ordered) with the value of each pixel representing the number of objects in it.
        Eigen::ArrayXi get_countmap() const;

        //! Get the associated Healpix_Base instance.
        const Healpix_Base& get_hp_base() const;

    private:
        Healpix_Base hp_base;
        unsigned int nside;
    };

}

#endif //CATANA_PIXELIZEDOBJECTCONTAINER_HPP
