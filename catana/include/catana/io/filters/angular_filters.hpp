#ifndef CATANA_ANGULAR_FILTERS_HPP
#define CATANA_ANGULAR_FILTERS_HPP

#include "../Filter.hpp"
#include <healpix_map.h>
#include <Eigen/Dense>


namespace catana { namespace io {

  //! Applies an angular mask (float healpix map)
  /*!
   * MASK==1: keep point at this pixel
   * MASK==0: remove point at this pixel
   */
  class AngularMaskFilter : public Filter {
  public:
    //! Construct from HEALPix map FITS file
    /*!
     * @param healpix_mask valid HEALPix FITS file, in RING format, with pixel values 1 (passes filter)
     * and 0 (remove points)
     */
    explicit AngularMaskFilter(std::string healpix_mask);

    //! Construct from HEALPix map array
    /*!
     * @param healpix_map valid HEALPix map, in RING format, with pixel values 1 (passes filter)
     * and 0 (remove points)
     */
    AngularMaskFilter(const Eigen::ArrayXf& healpix_map);

    //! Filtering function on Point. Returns true if point passes filter, false otherwise.
    bool filter(Point& point) override;

  private:
    Healpix_Map<float> map;
  };

}}
#endif //CATANA_ANGULAR_FILTERS_HPP
