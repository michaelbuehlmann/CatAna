//
// Created by Michael Bühlmann on 27/01/16.
//

#include "catana/tools/return_types.hpp"
#include "catana/besseltools.hpp"
#include <cmath>

Eigen::ArrayXXd compute_kln(unsigned short lmax, unsigned short nmax, double_t Rmax) {
    Eigen::ArrayXXd k_ln(lmax, nmax);
    for (int l = 0; l<lmax; ++l) {
        BesselZeros BZ(l);
        for (int n = 0; n<nmax; ++n) {
            k_ln(l, n) = (BZ[n]/Rmax);
        }
    }

    return k_ln;
}


KClkk::KClkk(unsigned short lmax, unsigned short nmax, double Rmax)
        : k_ln(compute_kln(lmax, nmax, Rmax)), c_ln(Eigen::ArrayXXd::Zero(lmax, nmax))
{}


KPkk::KPkk(unsigned short M, double L, unsigned short bin_number, bool logdist)
{
    double k_min = 2*M_PI/L;
    double k_max = std::sqrt(3) * M/2. * k_min;
    k = Eigen::ArrayXd::Zero(M);
    pkk = Eigen::ArrayXd::Zero(M);
    bin_count = Eigen::Array<unsigned int, Eigen::Dynamic, 1>::Zero(M);
    if(!logdist) {
        k_binedges = Eigen::ArrayXd::LinSpaced(bin_number+1, std::log(k_min), std::log(k_max));
        k_binedges.exp();
    } else {
        k_binedges = Eigen::ArrayXd::LinSpaced(bin_number+1, k_min, k_max);
    }
    for (int i = 0; i<bin_number; ++i) {
        k(i) = 0.5*(k_binedges(i)+k_binedges(i+1));
    }
}
