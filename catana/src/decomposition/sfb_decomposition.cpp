//
// Created by Michael Bühlmann on 22/01/16.
//

#include <catana/decomposition/sfb_decomposition.hpp>
//#include <catana/tools/SphericalHarmonicsLoader.hpp>
#include <boost/math/special_functions/spherical_harmonic.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <Eigen/Dense>

#include <iostream>

// TODO: logging/output class/library


using complex = std::complex<double_t>;

KClkk decomp_SFB(
        const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose, bool parallel)
{
    size_t container_size = size_t(std::distance(std::begin(objects), std::end(objects)));
    if(verbose) {
        std::cout << "SFB decomposition (raw method):"
                << " lmax=" << lmax
                << " nmax=" << nmax
                << " Rmax=" << rmax
                << std::endl;
        std::cout << "Catalog consists of " << container_size << " objects in a volume of " << window_volume << std::endl;
    }

    double_t norm_factor = std::sqrt(2/M_PI) * window_volume/container_size;
    KClkk kclkk(lmax, nmax, rmax);

    for (unsigned short l = 0; l<lmax; ++l) {
        if(verbose)
            std::cout << "\tl = " << l << ": Computing f_lmn ... ";
        Eigen::ArrayXXcd f_l_mn = Eigen::ArrayXXcd::Zero(l+1, nmax);

#pragma omp parallel if(parallel)
        {
            Eigen::ArrayXXcd f_l_mn_private = Eigen::ArrayXXcd::Zero(l+1, nmax);

            // Loop over all objects
#pragma omp for nowait
            for(ObjectContainer::const_iterator obj_it=objects.begin(); obj_it < objects.end(); ++obj_it) {

                // Compute j_ln for all n
                Eigen::ArrayXd j_l_n(nmax);

                for (unsigned short n = 0; n<nmax; ++n) {
                    j_l_n(n) = boost::math::sph_bessel(l, kclkk.k_ln(l, n)*obj_it->r);
                }

                // Compute Y_lm
                for (unsigned short m = 0; m<=l; ++m) {
                    complex y_lm = std::conj(boost::math::spherical_harmonic(l, m, obj_it->p.theta, obj_it->p.phi));

                    for (unsigned short n = 0; n<nmax; ++n) {
                        f_l_mn_private(m, n) += kclkk.k_ln(l, n)*j_l_n(n)*y_lm;
                    }
                }
            }
#pragma omp critical
            {
                f_l_mn += f_l_mn_private;
            }
        }
        if(verbose)
            std::cout << "Computing C_l(k,k) ... ";
        // Norming and computing C_l(k,k)
        f_l_mn *= norm_factor;
        for (unsigned short n=0; n<nmax; ++n){
            for (int m=-l; m<=l; ++m) {
                kclkk.c_ln(l,n) += std::pow(std::abs(f_l_mn(abs(m),n)),2);
            }
            kclkk.c_ln(l,n) /= (2*l+1);
        }
        if(verbose)
            std::cout << "Done.\n";
    }

    return kclkk;
}

KClkk decomp_SFB(const PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose, bool parallel)
{
    auto nside = pix_obj_cont.get_nside();
    auto npix = pix_obj_cont.size();

    if(verbose){
        std::cout << "SFB decomposition (reverse method):"
                << " lmax=" << lmax
                << " nmax=" << nmax
                << " NSide=" << nside
                << std::endl;
        std::cout << "Catalog consists of " << pix_obj_cont.get_nobjects() << " objects in a volume of " << window_volume << std::endl;
    }

    double_t norm_factor = std::sqrt(2/M_PI) * window_volume/pix_obj_cont.get_nobjects();
    KClkk kclkk(lmax, nmax, rmax);

    /////////////////////
    // Computing f_lmn //
    for(unsigned short l=0; l<lmax; ++l) {
        if(verbose)
            std::cout << "\tl = " << l << ": spherical harmonics ... ";

        // Spherical harmonics decomposition
        // TODO: save spherical harmonics in external file, load if needed.
        Eigen::MatrixXcd y_l_mi(l+1, npix);
#pragma omp parallel for if(parallel)
        for(int i=0; i<npix; ++i) {
            const pointing& p = pix_obj_cont[i].p;
            for(auto m=0; m<=l; ++m) {
                y_l_mi(m,i) = std::conj(boost::math::spherical_harmonic(l, m, p.theta, p.phi));
            }
        }

        // Bessel decomposition.
        if(verbose)
            std::cout << "Done. Bessel decomposition ... ";
        // Allocate space for f_lmn and f_ln
        Eigen::MatrixXcd f_l_mn = Eigen::MatrixXcd::Zero(l+1, nmax);
        Eigen::MatrixXd f_l_in = Eigen::MatrixXd::Zero(npix, nmax);

        // Loop over all pixels and do Bessel decomposition:
#pragma omp parallel for if(parallel)
        for (int i = 0; i<npix; ++i) {
            for (unsigned short n = 0; n<nmax; ++n) {
                const auto& k = kclkk.k_ln(l,n);
                for(const auto& r: pix_obj_cont[i]){
                    f_l_in(i, n) += boost::math::sph_bessel(l, k*r);
                }
                f_l_in(i, n) *= k;
            }
        }

        // Obtain full decomposition
        if(verbose)
            std::cout << "Done. Combining ... ";
        f_l_mn = y_l_mi*f_l_in;

        // Norming and computing C_l(k,k)
        if(verbose)
            std::cout << "Done. C_l(k,k) ... ";
        f_l_mn *= norm_factor;
        for (unsigned short n=0; n<nmax; ++n){
            for (int m=-l; m<=l; ++m) {
                kclkk.c_ln(l,n) += std::pow(std::abs(f_l_mn(abs(m),n)),2);
            }
            kclkk.c_ln(l,n) /= (2*l+1);
        }
        if(verbose)
            std::cout << "Done." << std::endl;
    }
    return kclkk;
}