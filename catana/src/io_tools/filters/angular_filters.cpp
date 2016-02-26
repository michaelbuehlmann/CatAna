//
// Created by Michael Bühlmann on 15/02/16.
//

#include <catana/io_tools/filters/angular_filters.hpp>
#include <healpix_cxx/healpix_map_fitsio.h>

namespace catana{ namespace io {

        AngularMaskFilter::AngularMaskFilter(std::string healpix_mask)
        {
            read_Healpix_map_from_fits(healpix_mask, map);
        }

        bool AngularMaskFilter::filter(Object& object)
        {
            return (map[map.ang2pix(object.p)]==1.f);
        }

}}