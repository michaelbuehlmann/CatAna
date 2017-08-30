#ifndef CATANA_SFB_RAW_HPP
#define CATANA_SFB_RAW_HPP

#include <catana/config.hpp>
#include "return_types.hpp"
#include "../points/PointContainer.hpp"


namespace catana {
  //! Do the SFB-decomposition of PointContainer (raw, brute-force method)
  KClkk _sfb_raw(const PointContainer& points, unsigned short lmax, unsigned short nmax, double rmax, bool store_flmn,
                 bool verbose, bool parallel, bool interpolated);
}
#endif //CATANA_SFB_RAW_HPP
