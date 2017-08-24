#ifndef CATANA_SFB_REVERSE_FFT_HPP
#define CATANA_SFB_REVERSE_FFT_HPP

#include <catana/config.hpp>
#include "return_types.hpp"
#include "../points/PixelizedPointContainer.hpp"


namespace catana {
  //! Do the SFB-decomposition of PixelizedPointContainer (with FFTs)
  KClkk _sfb_reverse_fft(
      const PixelizedPointContainer& pix_oc,
      unsigned short lmax, unsigned short nmax,
      double rmax, double window_volume, bool store_flmn = false, bool verbose = true,
      bool parallel = false, bool interpolated = false
  );
}
#endif //CATANA_SFB_REVERSE_FFT_HPP
