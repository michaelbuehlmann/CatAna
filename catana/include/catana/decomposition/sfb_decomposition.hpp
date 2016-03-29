//
// Created by Michael Bühlmann on 27/01/16.
//

#ifndef CATANA_APP_SFB_DECOMPOSITION_HPP
#define CATANA_APP_SFB_DECOMPOSITION_HPP

#include "../types.hpp"
#include "return_types.hpp"

namespace catana {
////////////////////////////////////////////////////////////////////////////////
// Convenience wrappers ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    //! Do the SFB-decomposition of the ObjectContainer (raw, brute-force method)
    KClkk sfb_decomposition(const ObjectContainer& objects,
            unsigned short lmax, unsigned short nmax,
            double rmax, double window_volume, bool store_flmn = false, bool verbose = true);

    //! Do the SFB-decomposition of the PixelizedObjectContainer (with FFT)
    KClkk sfb_decomposition(PixelizedObjectContainer& pix_obj_cont,
            unsigned short lmax, unsigned short nmax,
            double rmax, double window_volume, bool store_flmn = false, bool verbose = true);

////////////////////////////////////////////////////////////////////////////////
}
#endif //CATANA_APP_SFB_DECOMPOSITION_HPP
