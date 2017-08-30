#include <catana/decomposition/sfb_decomposition.hpp>
#include <catana/decomposition/sfb_raw.hpp>
#include <catana/decomposition/sfb_reverse_fft.hpp>


namespace catana {
//! For the spherical bessel function interpolation: #interpolation points per n
//  Total interpolation points: nmax*BESSELINTERPOLATIONPOINTS_PER_ZERO

////////////////////////////////////////////////////////////////////////////////
// Convenience wrappers ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


  KClkk sfb_decomposition(const PointContainer& points, unsigned short lmax, unsigned short nmax, double rmax, bool store_flmn,
                            bool verbose) {
    bool interpolated = (points.size() > BESSELINTERPOLATIONPOINTS_PER_ZERO);
    return _sfb_raw(points, lmax, nmax, rmax, store_flmn, verbose, true, interpolated);
  }


  KClkk sfb_decomposition(const PixelizedPointContainer& pix_obj_cont, unsigned short lmax, unsigned short nmax, double rmax,
                            bool store_flmn, bool verbose) {
    bool interpolated = (pix_obj_cont.get_npoints() > BESSELINTERPOLATIONPOINTS_PER_ZERO);
    return _sfb_reverse_fft(pix_obj_cont, lmax, nmax, rmax, store_flmn, verbose, true, interpolated);
  }

}