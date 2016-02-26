//
// Created by Michael Bühlmann on 27/01/16.
//

#ifndef CATANA_APP_DECOMP_OBJECTS_HPP
#define CATANA_APP_DECOMP_OBJECTS_HPP

#include <Eigen/Dense>
#include <string>
#include <ostream>
#include <vector>


//! Return Type of SFB decomposition methods
/*
 * k_ln contains the k at which C_l(k,k) is evaluated
 * c_ln contains the corresponding C_l(k,k)
 * (index 0: 0<l<lmax,
 *  index 1: 0<n<nmax)
 */
struct KClkk {
public:
    KClkk(unsigned short lmax, unsigned short nmax, double rmax);
    Eigen::ArrayXXd k_ln;
    Eigen::ArrayXXd c_ln;
    std::vector<Eigen::ArrayXXcd> f_lmn;
public:
    //! Save k_ln and c_ln to files '{filename_base}.(k_ln/c_ln) respectively. Rows are l, columns are n.
    void savetxt(std::string filename_base);
};


struct KPkk {
    KPkk(unsigned short M, double L, unsigned short bin_number = 40, bool logdist = true);
    Eigen::ArrayXd k;
    Eigen::ArrayXd pkk;
    Eigen::Array<unsigned int, Eigen::Dynamic, 1> bin_count;
    Eigen::ArrayXd k_binedges;
    //! k, pkk to file '{filename_base}.pkk' (each row of format (k_i, pkk_i))
    void savetxt(std::string filename_base);
};
#endif //CATANA_APP_DECOMP_OBJECTS_HPP
