//
// Created by Michael Bühlmann on 27/01/16.
//

#include "catana/tools/return_types.hpp"
#include "catana/besseltools/besselzeros.hpp"
#include <cmath>
#include <iomanip>
#include <fstream>


Eigen::ArrayXXd compute_kln(unsigned short lmax, unsigned short nmax, double_t rmax) {
    Eigen::ArrayXXd k_ln(lmax, nmax);
    for (unsigned short l = 0; l<lmax; ++l) {
        SphericalBesselZeros SBZ(l);
        for (int n = 0; n<nmax; ++n) {
            k_ln(l, n) = (SBZ[n]/rmax);
        }
    }

    return k_ln;
}


KClkk::KClkk(unsigned short lmax, unsigned short nmax, double rmax)
        : k_ln(compute_kln(lmax, nmax, rmax)), c_ln(Eigen::ArrayXXd::Zero(lmax, nmax))
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

template <class M, class O>
void output_txt(M& array, O& ostream){
    ostream.precision(18);
    ostream.setf(std::ios::scientific);
    for(unsigned short i=0; i<array.rows(); ++i){
        for(unsigned short j=0; j<array.cols(); ++j){
            ostream << array(i,j);
            if(j!=array.cols()-1)
                ostream << " ";
        }
        ostream << std::endl;
    }
};

void KClkk::savetxt(std::string filename_base)
{
    std::ofstream fs_k_ln, fs_c_ln;
    fs_k_ln.open(filename_base + ".k_ln", std::ios::out | std::ios::trunc);
    output_txt(k_ln, fs_k_ln);
    fs_k_ln.close();

    fs_c_ln.open(filename_base + ".c_ln", std::ios::out | std::ios::trunc);
    output_txt(c_ln, fs_c_ln);
    fs_c_ln.close();
}

void KPkk::savetxt(std::string filename_base)
{
    std::ofstream fs_kpkk;
    fs_kpkk.open(filename_base.append(".kpkk"), std::ios::out | std::ios::trunc);
    for(unsigned short i=0; i<k.rows(); ++i){
        fs_kpkk << k(i) << " " << pkk(i) << std::endl;
    }
    fs_kpkk.close();
}
