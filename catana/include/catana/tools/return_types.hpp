//
// Created by Michael Bühlmann on 27/01/16.
//

#ifndef CATANA_APP_DECOMP_OBJECTS_HPP
#define CATANA_APP_DECOMP_OBJECTS_HPP

#include <Eigen/Dense>
//! Return Type of SFB decomposition methods
/*
 * k_ln contains the k at which C_l(k,k) is evaluated
 * c_ln contains the corresponding C_l(k,k)
 * (index 0: 0<l<lmax,
 *  index 1: 0<n<nmax)
 */
struct KClkk {
    KClkk(unsigned short lmax, unsigned short nmax, double Rmax);
    Eigen::ArrayXXd k_ln;
    Eigen::ArrayXXd c_ln;
};


struct KPkk {
    KPkk(unsigned short M, double L, unsigned short bin_number = 40, bool logdist = true);
    Eigen::ArrayXd k;
    Eigen::ArrayXd pkk;
    Eigen::Array<unsigned int, Eigen::Dynamic, 1> bin_count;
    Eigen::ArrayXd k_binedges;
};
#endif //CATANA_APP_DECOMP_OBJECTS_HPP
