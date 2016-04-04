//
// Created by Michael Bühlmann on 23/02/16.
//

#include <catana/decomposition/sfb_reverse.hpp>
#include <catana/config.hpp>
#include <catana/besseltools.hpp>


#include <healpix_base.h>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_sf_legendre.h>

namespace catana {
    using complex = std::complex<double>;

    KClkk _sfb_reverse(const PixelizedObjectContainer& pix_oc,
            unsigned short lmax, unsigned short nmax,
            double rmax, double window_volume, bool store_flmn, bool verbose, bool parallel, bool interpolated)
    {
        auto nside = pix_oc.get_nside();
        auto npix = pix_oc.size();

        if (verbose) {
            std::cout << "SFB decomposition (reverse method):"
                    << " lmax=" << lmax
                    << " nmax=" << nmax
                    << " NSide=" << nside
                    << std::endl;
            std::cout << "Catalog consists of " << pix_oc.get_nobjects() << " objects in a volume of " << window_volume
                    << std::endl;
        }

        // Turn off GSL Error handler
        auto gsl_error_handler_old = gsl_set_error_handler_off();

        // Compute normalization factor and set up KClkk
        double_t norm_factor = std::sqrt(2/M_PI)*window_volume/pix_oc.get_nobjects();
        KClkk kclkk(lmax, nmax, rmax);

        // This will be a nullptr if !interpolated, otherwise we will assign it within the l-for loop
        std::unique_ptr<besseltools::SBesselInterpolator> sblu;
        std::function<double(double)> sph_bessel_l;

        // Actual decomposition
        for (unsigned short l = 0; l<lmax; ++l) {

            if (verbose) {
                std::cout << "\tl = " << l << " ...";
                std::cout.flush();
            }

            if (interpolated) {
                sblu.reset(new besseltools::SBesselInterpolator(
                        besseltools::SBesselInterpolator_uptozero(l, nmax, BESSELINTERPOLATIONPOINTS_PER_ZERO*nmax)
                ));
                sph_bessel_l = [&](const double& z) { return sblu->operator()(z); };
            } else {
                sph_bessel_l = [&](const double& z) { return gsl_sf_bessel_jl(l, z); };
            }

            // Allocate space for spherical harmonics
            Eigen::MatrixXcd y_l_mi(l+1, npix);
            // Allocate space for f_lmn and f_ln
            Eigen::MatrixXcd f_l_mn = Eigen::MatrixXcd::Zero(l+1, nmax);
            Eigen::MatrixXd f_l_in = Eigen::MatrixXd::Zero(npix, nmax);

#pragma omp parallel if(parallel)
            {
                // Spherical harmonics decomposition
                // TODO: save spherical harmonics in external file, load if needed.
#pragma omp for nowait
                for (int i = 0; i<npix; ++i) {
                    const pointing& p = pix_oc[i].p;
                    for (unsigned short m = 0; m<=l; ++m) {
                        double gsl_sphPlm = gsl_sf_legendre_sphPlm(l,m,std::cos(p.theta));
                        y_l_mi(m, i) = std::complex<double>(gsl_sphPlm*std::cos(-m*p.phi), gsl_sphPlm*std::sin(-m*p.phi));
                    }
                }

                // Loop over all pixels and do Bessel decomposition:
#pragma omp for
                for (int i = 0; i<npix; ++i) {
                    for (unsigned short n = 0; n<nmax; ++n) {
                        const auto& k = kclkk.k_ln(l, n);
                        for (const auto& r: pix_oc[i]) {
                            f_l_in(i, n) += sph_bessel_l(k*r);
                        }
                    }
                }
            }
            // Obtain full decomposition
            f_l_mn = y_l_mi*f_l_in;

            // Norming and computing C_l(k,k)
            for (unsigned short n = 0; n<nmax; ++n) {
                for (int m = -l; m<=l; ++m) {
                    kclkk.c_ln(l, n) += std::pow(std::abs(f_l_mn(abs(m), n)), 2);
                }
                kclkk.c_ln(l, n) *= std::pow(norm_factor*kclkk.k_ln(l, n), 2)/(2*l+1);
            }

            if (store_flmn) {
                for (unsigned short n = 0; n<nmax; ++n) {
                    f_l_mn.col(n) *= kclkk.k_ln(l, n)*norm_factor;
                }
                kclkk.f_lmn.push_back(f_l_mn);
            }

            if (verbose)
                std::cout << "Done." << std::endl;
        }

        // Reset old GSL error handler
        gsl_set_error_handler(gsl_error_handler_old);
        return kclkk;
    }
}