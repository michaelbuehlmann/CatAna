#ifndef CATANA_PIXELIZEDPOINTCONTAINER_HPP
#define CATANA_PIXELIZEDPOINTCONTAINER_HPP

#include "Point.hpp"
#include "PointContainer.hpp"
#include <healpix_base.h>
#include <pointing.h>
#include <vector>
#include <Eigen/Dense>


namespace catana {

  //! Pixel on spherical shell containing points at different distances r: std::vector<double>
  class PixelPoints : public std::vector<double> {
  public:
    pointing p; //!< the angular position of the pixel: a HEALPix pointing
  };

  //! A HEALPix Map of PixelPoints
  /*!
   * \verbatim embed:rst:leading-asterisk
   * .. warning::
   *
   *    DO NOT CHANGE SIZE (push_back, resize, etc) of underlying std::vector once constructed!
   * \endverbatim
   */
  class PixelizedPointContainer : public std::vector<PixelPoints> {
  public:
    //! Construct for given nside.
    /*!
     * The instance will have size nside2npix(nside) (see HEALPix documentation).
     * Each PixelPoints will have size 0.
     */
    PixelizedPointContainer(unsigned int nside);

    //! Construct for given nside from an existing PointContainer.
    /*!
     * The instance will have size nside2npix(nside) (see HEALPix documentation)
     * Each Pixel is assigned the according Points from PointContainer
     */
    PixelizedPointContainer(unsigned int nside, const PointContainer& oc);

    //! Add point to PixelizedPointContainer
    void add_point(const Point&);

    //! Get the NSide
    unsigned int get_nside() const;

    //! Get number of points stored within PixelizedPointContainer (i.e. sum points over all pixels)
    size_t get_npoints() const;

    //! Get a HEALPix map (RING ordered) with the value of each pixel representing the number of points in it.
    Eigen::ArrayXi get_countmap() const;

    //! Get the associated Healpix_Base instance.
    const Healpix_Base& get_hp_base() const;

  private:
    Healpix_Base hp_base;
    unsigned int nside;
  };

}

#endif //CATANA_PIXELIZEDPOINTCONTAINER_HPP
