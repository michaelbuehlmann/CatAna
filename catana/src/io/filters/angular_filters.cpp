#include <catana/io/filters/angular_filters.hpp>
#include <healpix_map_fitsio.h>
#include <healpix_base.h>

namespace catana { namespace io {

  AngularMaskFilter::AngularMaskFilter(std::string healpix_mask) {
    read_Healpix_map_from_fits(healpix_mask, map);
  }

  bool AngularMaskFilter::filter(Point& point) {
    return (map[map.ang2pix(point.p)] == 1.f);
  }

  AngularMaskFilter::AngularMaskFilter(const Eigen::ArrayXf& healpix_map) {
    unsigned int nside = std::sqrt(healpix_map.rows()/12);
    if(12*nside*nside != healpix_map.rows())
      throw std::runtime_error("healpix_map is not a valid map (check size!)");

    const arr<float> data(const_cast<float*>(healpix_map.data()), healpix_map.rows());
    map = Healpix_Map<float>(data, Healpix_Ordering_Scheme::RING);
  }

}}