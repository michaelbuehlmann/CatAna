#ifndef CATANA_APP_SFB_DECOMPOSITION_HPP
#define CATANA_APP_SFB_DECOMPOSITION_HPP

#include "../points/PointContainer.hpp"
#include "../points/PixelizedPointContainer.hpp"
#include "return_types.hpp"


namespace catana {
////////////////////////////////////////////////////////////////////////////////
// Convenience wrappers ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  //! compute the SFB transform on a PointContainer
  /*!
   * Warning:
   *     This method uses the slow brute-force method. Consider using a `PixelizedPointContainer` instead
   *
   * @param point_container data to analyze
   * @param lmax largest multipole to compute
   * @param nmax largest k-index to compute
   * @param rmax maximal radius of data
   * @param store_flmn if ``true`` the output will contain the f_lmn coefficients, otherwise only the C_ln
   * @param verbose if ``true`` print additional logging information to stdout
   *
   * @return SFB components including the k at which they were computed and the C_l(k). If store_flmn, will also save the
   *     f_lm(k) components
   */
  KClkk sfb_decomposition(const PointContainer& points, unsigned short lmax, unsigned short nmax, double rmax, bool store_flmn,
                            bool verbose);

  //! compute the SFB transform on a PixelizedPointContainer
  /*!
   * This method makes use of the fast pixelized decomposition scheme.
   *
   * @param point_container data to analyze
   * @param lmax largest multipole to compute
   * @param nmax largest k-index to compute
   * @param rmax maximal radius of data
   * @param store_flmn if ``true`` the output will contain the f_lmn coefficients, otherwise only the C_ln
   * @param verbose if ``true`` print additional logging information to stdout
   *
   * @return SFB components including the k at which they were computed and the C_l(k). If store_flmn, will also save the
   *     f_lm(k) components
   */
  KClkk sfb_decomposition(const PixelizedPointContainer& pix_obj_cont, unsigned short lmax, unsigned short nmax, double rmax,
                            bool store_flmn, bool verbose);

////////////////////////////////////////////////////////////////////////////////
}
#endif //CATANA_APP_SFB_DECOMPOSITION_HPP
