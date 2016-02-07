//
// Created by Michael Bühlmann on 27/01/16.
//

#ifndef CATANA_APP_SFB_DECOMPOSITION_HPP
#define CATANA_APP_SFB_DECOMPOSITION_HPP

#include "catana/tools/return_types.hpp"
#include "catana/tools/object_types.hpp"

KClkk decomp_SFB_raw(
        const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume
);

KClkk decomp_SFB_reverse(const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume,
        unsigned int nside
);


#endif //CATANA_APP_SFB_DECOMPOSITION_HPP
