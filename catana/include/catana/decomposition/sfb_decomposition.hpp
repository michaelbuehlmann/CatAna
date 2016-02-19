//
// Created by Michael Bühlmann on 27/01/16.
//

#ifndef CATANA_APP_SFB_DECOMPOSITION_HPP
#define CATANA_APP_SFB_DECOMPOSITION_HPP

#include "../types.hpp"

KClkk decomp_SFB(
        const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose=true, bool parallel=false
);

KClkk decomp_SFB(
        const PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose=true, bool parallel=false
);

KClkk decomp_SFB_FFT(
        const PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose=true, bool parallel=false
);

//KClkk decomp_SFB_FFT_v2(
//        const PixelizedObjectContainer& pix_obj_cont,
//        unsigned short lmax, unsigned short nmax,
//        double rmax, double window_volume, bool verbose=true, bool parallel=false
//);
//
//KClkk decomp_SFB_FFT_v3(
//        const PixelizedObjectContainer& pix_obj_cont,
//        unsigned short lmax, unsigned short nmax,
//        double rmax, double window_volume, bool verbose=true, bool parallel=false
//);
#endif //CATANA_APP_SFB_DECOMPOSITION_HPP
