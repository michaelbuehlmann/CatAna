//
// Created by Michael Bühlmann on 22/01/16.
//

#include <catana/decomposition/sfb_decomposition.hpp>
#include <catana/decomposition/sfb_raw.hpp>
#include <catana/decomposition/sfb_reverse.hpp>
#include <catana/decomposition/sfb_reverse_fft.hpp>

#include <catana/config.hpp>

//! For the spherical bessel function interpolation: #interpolation points per n
//  Total interpolation points: nmax*BESSELINTERPOLATIONPOINTS_PER_ZERO

////////////////////////////////////////////////////////////////////////////////
// Convenience wrappers ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

KClkk sfb_decomposition(const ObjectContainer& objects, unsigned short lmax, unsigned short nmax, double rmax,
        double window_volume, bool verbose)
{
    bool interpolated = (objects.size()>BESSELINTERPOLATIONPOINTS_PER_ZERO);
    return _sfb_raw(objects, lmax, nmax, rmax, window_volume, verbose, true, interpolated);
}

KClkk sfb_decomposition(PixelizedObjectContainer& pix_obj_cont, unsigned short lmax, unsigned short nmax, double rmax,
        double window_volume, bool verbose)
{
    bool interpolated = (pix_obj_cont.get_nobjects()>BESSELINTERPOLATIONPOINTS_PER_ZERO);
    return _sfb_reverse_fft(pix_obj_cont, lmax, nmax, rmax, window_volume, verbose, true, interpolated);
}