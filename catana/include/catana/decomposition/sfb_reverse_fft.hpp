#ifndef CATANA_SFB_REVERSE_FFT_HPP
#define CATANA_SFB_REVERSE_FFT_HPP

#include <catana/config.hpp>
#include "return_types.hpp"
#include "../points/PixelizedPointContainer.hpp"


namespace catana {
  //! Do the SFB-decomposition of PixelizedPointContainer (with FFTs)
  KClkk _sfb_reverse_fft(const PixelizedPointContainer& pix_oc, unsigned short lmax, unsigned short nmax, double rmax,
                         bool store_flmn, bool verbose, bool parallel, bool interpolated);
}
#endif //CATANA_SFB_REVERSE_FFT_HPP
