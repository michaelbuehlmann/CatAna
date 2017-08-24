#ifndef CATANA_RETURN_TYPES
#define CATANA_RETURN_TYPES

#include <Eigen/Dense>
#include <string>
#include <ostream>
#include <vector>


namespace catana {

//! Return Type of SFB decomposition methods
/*!
 * k_ln contains the k at which C_l(k,k) is evaluated
 * c_ln contains the corresponding C_l(k,k)
 * (index 0: 0<l<lmax,
 *  index 1: 0<n<nmax)
 */
  struct KClkk {
  public:
    //! Construct KClkk for given lmax and nmax
    /*!
     * This will initialize k_ln to the Zeros of the spherical bessel functions normalized with rmax.
     * The c_ln and f_lmn will be set to 0.
     * @param lmax k_ln and c_ln will contain space for l in [0,lmax)
     * @param nmax k_ln and c_ln will contain space for n in [0,nmax)
     * @param rmax the radius at which the boundary condition f(rmax)=0 is assumed. Will normalize the k_ln at which
     * the c_ln are evaluated.
     */
    KClkk(unsigned short lmax, unsigned short nmax, double rmax);

    Eigen::ArrayXXd k_ln;  //!< the k's at which c_ln are evaluated
    Eigen::ArrayXXd c_ln;  //!< the C_l(k_n, k_n)
    std::vector<Eigen::ArrayXXcd> f_lmn;  //!< the coefficients from which the c_ln were computed
  public:
    //! Save k_ln and c_ln to files '{filename_base}.(k_ln/c_ln) respectively. Rows are l, columns are n.
    void savetxt(std::string filename_base);
  };
}
#endif //CATANA_RETURN_TYPES
