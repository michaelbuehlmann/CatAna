#ifndef CATANA_SFB_RAW_HPP
#define CATANA_SFB_RAW_HPP

#include <catana/config.hpp>
#include "return_types.hpp"
#include "../points/PointContainer.hpp"


namespace catana {
  //! Do the SFB-decomposition of PointContainer (raw, brute-force method)
  KClkk _sfb_raw(
      const PointContainer& points,
      unsigned short lmax, unsigned short nmax,
      double rmax, double window_volume, bool store_flmn = false, bool verbose = true,
      bool parallel = false, bool interpolated = false
  );
}
#endif //CATANA_SFB_RAW_HPP
