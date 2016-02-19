//
// Created by Michael Bühlmann on 27/01/16.
//

#ifndef CATANA_APP_SFB_DECOMPOSITION_HPP
#define CATANA_APP_SFB_DECOMPOSITION_HPP

#include "../types.hpp"

////////////////////////////////////////////////////////////////////////////////
// Convenience wrappers ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

KClkk compute_SFB(const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose=true);

KClkk compute_SFB(PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose=true);

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Implementation //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//! Decomposition of ObjectContainer
KClkk _decomp_SFB(
        const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose = true, bool parallel = false, bool interpolated = false
);

//! Decomposition of PixelizedObjectContainer
KClkk _decomp_SFB(
        const PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose = true, bool parallel = false, bool interpolated = false
);

//! Decomposition of PixelizedObjectContainer using FFTs
KClkk _decomp_SFB_FFT(
        const PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose = true, bool parallel = false, bool interpolated = false
);
#endif //CATANA_APP_SFB_DECOMPOSITION_HPP
