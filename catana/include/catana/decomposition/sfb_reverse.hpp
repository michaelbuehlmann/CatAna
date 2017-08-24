#ifndef CATANA_SFB_REVERSE_HPP
#define CATANA_SFB_REVERSE_HPP

#include <catana/config.hpp>
#include "return_types.hpp"
#include "../points/PixelizedPointContainer.hpp"


namespace catana {
  //! Do the SFB-decomposition of PixelizedPointContainer (without FFTs)
  KClkk _sfb_reverse(
      const PixelizedPointContainer& pix_oc,
      unsigned short lmax, unsigned short nmax,
      double rmax, double window_volume, bool store_flmn = false, bool verbose = true,
      bool parallel = false, bool interpolated = false
  );
}

#endif //CATANA_SFB_REVERSE_HPP
