//
// Created by Michael Bühlmann on 27/01/16.
//

#include <catana/decomposition/return_types.hpp>
#include <catana/besseltools.hpp>
#include <cmath>
#include <iomanip>
#include <fstream>

namespace catana {

    Eigen::ArrayXXd compute_kln(unsigned short lmax, unsigned short nmax, double_t rmax)
    {
        Eigen::ArrayXXd k_ln(lmax, nmax);
        for (unsigned short l = 0; l<lmax; ++l) {
            besseltools::SphericalBesselZeros SBZ(l);
            for (int n = 0; n<nmax; ++n) {
                k_ln(l, n) = (SBZ[n]/rmax);
            }
        }

        return k_ln;
    }

    KClkk::KClkk(unsigned short lmax, unsigned short nmax, double rmax)
            :k_ln(compute_kln(lmax, nmax, rmax)), c_ln(Eigen::ArrayXXd::Zero(lmax, nmax)) { }

    template<class M, class O>
    void output_txt(M& array, O& ostream)
    {
        ostream.precision(18);
        ostream.setf(std::ios::scientific);
        for (unsigned short i = 0; i<array.rows(); ++i) {
            for (unsigned short j = 0; j<array.cols(); ++j) {
                ostream << array(i, j);
                if (j!=array.cols()-1)
                    ostream << " ";
            }
            ostream << std::endl;
        }
    };

    void KClkk::savetxt(std::string filename_base)
    {
        std::ofstream fs_k_ln, fs_c_ln;
        fs_k_ln.open(filename_base+".k_ln", std::ios::out | std::ios::trunc);
        output_txt(k_ln, fs_k_ln);
        fs_k_ln.close();

        fs_c_ln.open(filename_base+".c_ln", std::ios::out | std::ios::trunc);
        output_txt(c_ln, fs_c_ln);
        fs_c_ln.close();
    }
}