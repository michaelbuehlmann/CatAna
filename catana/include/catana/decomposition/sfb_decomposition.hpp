//
// Created by Michael Bühlmann on 27/01/16.
//

#ifndef CATANA_APP_SFB_DECOMPOSITION_HPP
#define CATANA_APP_SFB_DECOMPOSITION_HPP

#include "../types.hpp"

namespace catana {
////////////////////////////////////////////////////////////////////////////////
// Convenience wrappers ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    KClkk sfb_decomposition(const ObjectContainer& objects,
            unsigned short lmax, unsigned short nmax,
            double rmax, double window_volume, bool store_flmn = false, bool verbose = true);

    KClkk sfb_decomposition(PixelizedObjectContainer& pix_obj_cont,
            unsigned short lmax, unsigned short nmax,
            double rmax, double window_volume, bool store_flmn = false, bool verbose = true);

////////////////////////////////////////////////////////////////////////////////
}
#endif //CATANA_APP_SFB_DECOMPOSITION_HPP
