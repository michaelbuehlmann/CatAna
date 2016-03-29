//
// Created by Michael Bühlmann on 23/02/16.
//

#ifndef CATANA_SFB_RAW_HPP
#define CATANA_SFB_RAW_HPP

#include <catana/config.hpp>
#include "return_types.hpp"
#include "../types.hpp"

namespace catana {
    //! Do the SFB-decomposition of ObjectContainer (raw, brute-force method)
    KClkk _sfb_raw(
            const ObjectContainer& objects,
            unsigned short lmax, unsigned short nmax,
            double rmax, double window_volume, bool store_flmn = false, bool verbose = true,
            bool parallel = false, bool interpolated = false
    );
}
#endif //CATANA_SFB_RAW_HPP
