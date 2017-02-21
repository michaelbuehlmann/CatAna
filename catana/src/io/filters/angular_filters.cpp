#include <catana/io/filters/angular_filters.hpp>
#include <healpix_map_fitsio.h>


namespace catana { namespace io {

  AngularMaskFilter::AngularMaskFilter(std::string healpix_mask) {
    read_Healpix_map_from_fits(healpix_mask, map);
  }

  bool AngularMaskFilter::filter(Object& object) {
    return (map[map.ang2pix(object.p)] == 1.f);
  }

}}