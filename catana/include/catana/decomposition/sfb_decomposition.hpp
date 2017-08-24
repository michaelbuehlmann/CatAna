#ifndef CATANA_APP_SFB_DECOMPOSITION_HPP
#define CATANA_APP_SFB_DECOMPOSITION_HPP

#include "../points/PointContainer.hpp"
#include "../points/PixelizedPointContainer.hpp"
#include "return_types.hpp"


namespace catana {
////////////////////////////////////////////////////////////////////////////////
// Convenience wrappers ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  //! Do the SFB-decomposition of the PointContainer (raw, brute-force method)
  KClkk sfb_decomposition(const PointContainer& points,
                          unsigned short lmax, unsigned short nmax,
                          double rmax, double window_volume, bool store_flmn = false, bool verbose = true);

  //! Do the SFB-decomposition of the PixelizedPointContainer (with FFT)
  KClkk sfb_decomposition(PixelizedPointContainer& pix_obj_cont,
                          unsigned short lmax, unsigned short nmax,
                          double rmax, double window_volume, bool store_flmn = false, bool verbose = true);

////////////////////////////////////////////////////////////////////////////////
}
#endif //CATANA_APP_SFB_DECOMPOSITION_HPP
