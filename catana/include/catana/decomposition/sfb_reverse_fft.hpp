//
// Created by Michael Bühlmann on 23/02/16.
//

#ifndef CATANA_SFB_REVERSE_FFT_HPP
#define CATANA_SFB_REVERSE_FFT_HPP

#include <catana/config.hpp>
#include "../types.hpp"

KClkk _sfb_reverse_fft(
        const PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool store_flmn = false, bool verbose = true,
        bool parallel = false, bool interpolated = false
);


#endif //CATANA_SFB_REVERSE_FFT_HPP
