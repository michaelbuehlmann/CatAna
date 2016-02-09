//
// Created by Michael Bühlmann on 22/01/16.
//

#include <catana/decomposition/sfb_decomposition.hpp>
#include <boost/math/special_functions/spherical_harmonic.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <healpix_cxx/healpix_base.h>

#include <iostream>

// TODO: logging/output class/library


using complex = std::complex<double_t>;

KClkk decomp_SFB_raw(
        const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume
) {
    std::cout << "SFB decomposition (raw method):"
            << " lmax=" << lmax
            << " nmax=" << nmax
            << " Rmax=" << rmax
            << std::endl;
    size_t container_size = size_t(std::distance(std::begin(objects), std::end(objects)));
    std::cout << "Catalog consists of " << container_size << " objects in a volume of " << window_volume << std::endl;

    double_t norm_factor = std::sqrt(2/M_PI) * window_volume/container_size;
    KClkk kclkk(lmax, nmax, rmax);

    for (unsigned short l = 0; l<lmax; ++l) {
        std::cout << "\tl = " << l << ": Computing f_lmn ... ";
        Eigen::ArrayXXcd f_l_mn = Eigen::ArrayXXcd::Zero(l+1, nmax);

        // Loop over all objects
        for(auto object: objects) {
            Eigen::ArrayXd j_l_n(nmax);
            for(unsigned short n=0; n<nmax; ++n) {
                j_l_n(n) = boost::math::sph_bessel(l, kclkk.k_ln(l,n)*object.r);
            }
            for (unsigned short m = 0; m<=l; ++m) {
                complex y_lm = std::conj(boost::math::spherical_harmonic(l, m, object.p.theta, object.p.phi));

                for(unsigned short n=0; n<nmax; ++n) {
                    f_l_mn += kclkk.k_ln(l,n)*j_l_n(n)*y_lm;
                }
            }
        }

        // Norming and computing C_l(k,k)
        std::cout << "Computing C_l(k,k) ... ";
        f_l_mn *= norm_factor;
        for (unsigned short n=0; n<nmax; ++n){
            for (int m=-l; m<=lmax; ++m) {
                kclkk.c_ln(l,n) += std::pow(std::abs(f_l_mn(abs(m),n)),2);
            }
            kclkk.c_ln(l,n) /= (2*l+1);
        }
        std::cout << "Done." << std::endl;
    }

    return kclkk;
}

KClkk decomp_SFB_reverse(const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume,
        unsigned int nside
) {
    std::cout << "SFB decomposition (reverse method):"
            << " lmax=" << lmax
            << " nmax=" << nmax
            << " NSide=" << nside
            << std::endl;
    size_t container_size = size_t(std::distance(std::begin(objects), std::end(objects)));
    std::cout << "Catalog consists of " << container_size << " objects in a volume of " << window_volume << std::endl;

    double_t norm_factor = std::sqrt(2/M_PI) * window_volume/container_size;
    KClkk kclkk(lmax, nmax, rmax);
    std::cout << "Assigning objects to HEALPix grid (NSide " << nside << ") ... ";

    // Assigning object to HEALPix Cells
    Healpix_Base hp_base(nside, RING, SET_NSIDE);
    std::vector<std::vector<double>> object_in_pix(hp_base.Npix());
    for(auto obj: objects) {
        object_in_pix[hp_base.ang2pix(obj.p)].push_back(obj.r);
    }

    std::cout << "Done." << std::endl;

    // Computing f_lmn
    for(int l=0; l<lmax; ++l) {

        std::cout << "\tl = " << l << ": spherical harmonics ... ";
        // Spherical harmonics decomposition
        // TODO: save spherical harmonics in external file, load if needed.
        Eigen::MatrixXcd y_l_mi(l+1, hp_base.Npix());
        for(auto i=0; i<hp_base.Npix(); ++i) {
            pointing p = hp_base.pix2ang(i);
            for(auto m=0; m<=l; ++m) {
                y_l_mi(m,i) = std::conj(boost::math::spherical_harmonic(l, m, p.theta, p.phi));
            }
        }
        std::cout << "Bessel decomposition ... ";
        // Allocate space for f_lmn and f_ln
        Eigen::MatrixXcd f_l_mn = Eigen::MatrixXcd::Zero(l+1, nmax);
        Eigen::MatrixXd f_l_in = Eigen::MatrixXd::Zero(hp_base.Npix(), nmax);

        // Loop over all pixels and do Bessel decomposition:
        for (auto i = 0; i<hp_base.Npix(); ++i) {
            for (unsigned short n = 0; n<nmax; ++n) {
                auto k = kclkk.k_ln(l,n);
                for(auto r: object_in_pix[i]){
                    f_l_in(i, n) += boost::math::sph_bessel(l, k*r);
                }
                f_l_in(i, n) *= k;
            }
        }
        std::cout << "full decomposition ... ";
        // Obtain full decomposition
        f_l_mn = y_l_mi*f_l_in;

        std::cout << "C_l(k,k) ... ";
        // Norming and computing C_l(k,k)
        f_l_mn *= norm_factor;
        for (unsigned short n=0; n<nmax; ++n){
            for (int m=-l; m<=lmax; ++m) {
                kclkk.c_ln(l,n) += std::pow(std::abs(f_l_mn(abs(m),n)),2);
            }
            kclkk.c_ln(l,n) /= (2*l+1);
        }
        std::cout << "Done." << std::endl;
    }
    return kclkk;
}