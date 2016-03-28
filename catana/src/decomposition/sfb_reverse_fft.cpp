//
// Created by Michael Bühlmann on 23/02/16.
//

#include <catana/decomposition/sfb_reverse_fft.hpp>
#include <catana/config.hpp>
#include <catana/besseltools.hpp>


#include <healpix_base.h>
#include <cmath>
#include <gsl/gsl_sf_legendre.h>
#include <gsl/gsl_sf_bessel.h>
#include <fftw3.h>

namespace catana {
    using complex = std::complex<double>;

//! A class holding information of a healpix ring, along with pointers to the actual map, the FFT-map and the FFTW-routine.
    struct RingInfo {
        RingInfo(int npix, const PixelObjects* const pixelobj1, double* map_i, complex* fft_map_i)
                :npix(npix), start_pixobjs(pixelobj1), phase(pixelobj1->p.phi), costheta(std::cos(pixelobj1->p.theta)), map(map_i),
                 fft_map(fft_map_i)
        {
            fft_forward = fftw_plan_dft_r2c_1d(npix, this->map, (fftw_complex*) this->fft_map, FFTW_MEASURE);
        }

        const int npix;
        const PixelObjects* const start_pixobjs;
        const double phase;
        const double costheta;
        double* map;
        complex* fft_map;

        fftw_plan fft_forward;
    };

//! The main computational part
    inline void sfb_ringdecomp_kernel(const unsigned short& l, double& k_n,
            const PixelizedObjectContainer& pix_oc,
            const std::function<double(double)>& sbessel_fct, Eigen::MatrixXcd& f_lmn,
            const complex* ylm_i,
            RingInfo& ring_info);

//! This function initialized and runs through the sfb decomposition using the reverse_fft-method.
    KClkk _sfb_reverse_fft(
            const PixelizedObjectContainer& pix_oc,
            unsigned short lmax, unsigned short nmax,
            double rmax, double window_volume, bool store_flmn, bool verbose, bool parallel, bool interpolated
    )
    {
        double_t norm_factor = std::sqrt(2/M_PI)*window_volume/pix_oc.get_nobjects();
        KClkk kclkk(lmax, nmax, rmax);

        auto nside = pix_oc.get_nside();
        auto npix = pix_oc.size();
        auto nrings = 4*nside-1;
        auto hp_base = pix_oc.get_hp_base();

        if (verbose) {
            std::cout << "SFB decomposition (reverse method):"
                    << " lmax=" << lmax
                    << " nmax=" << nmax
                    << " NSide=" << nside
                    << std::endl;
            std::cout << "Catalog consists of " << pix_oc.get_nobjects() << " objects in a volume of " << window_volume
                    << std::endl;
        }

        double* map = (double*) fftw_malloc(sizeof(double)*npix);
        complex* fft_map = (complex*) fftw_malloc(sizeof(complex)*(nrings+npix/2));

        std::vector<RingInfo> ring_info_container;
        ring_info_container.reserve(nrings);

        // Prepare FFTW plans.
        for (int i = 0; i<nrings; ++i) {
            int rstart, rpix;
            bool shifted;
            hp_base.get_ring_info_small(i+1, rstart, rpix, shifted);  // Healpix ring enumeration starts at 1
            ring_info_container.push_back(
                    RingInfo(
                            rpix, &pix_oc[rstart],
                            &map[rstart], &fft_map[rstart/2+i]
                    )
            );
        }

        // This will be a nullptr if !interpolated, otherwise we will assign it within the l-for loop
        std::unique_ptr<besseltools::SBesselInterpolator> sblu;
        std::function<double(double)> sph_bessel_l;

        // Actual decomposition
        for (unsigned short l = 0; l<lmax; ++l) {

            if (verbose) {
                std::cout << "\tl = " << l << " ... ";
                std::cout.flush();
            }

            if (store_flmn) {
                kclkk.f_lmn.push_back(Eigen::ArrayXXcd(l+1, nmax));
            }

            // Instantiate an interpolation routine for spherical bessels if asked for. Otherwise use plain function.
            if (interpolated) {
                sblu.reset(new besseltools::SBesselInterpolator(
                        besseltools::SBesselInterpolator_uptozero(l, nmax, BESSELINTERPOLATIONPOINTS_PER_ZERO*nmax)
                ));
                sph_bessel_l = [&](const double& z) { return sblu->operator()(z); };
            } else {
                sph_bessel_l = [&](const double& z) { return gsl_sf_bessel_jl(l, z); };
            }


            // Precompute spherical harmonics
            Eigen::Array<complex, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> ylm_im(nrings, l+1);
#pragma omp parallel for if(parallel) schedule(dynamic, 5)
            for (int i = 0; i<nrings; ++i) {
                for (unsigned short m = 0; m<=l; ++m) {
                    ylm_im(i, m) = std::polar(gsl_sf_legendre_sphPlm(l,m,ring_info_container[i].costheta), -m*ring_info_container[i].phase);
                }
            }

            for (unsigned short n = 0; n<nmax; ++n) {
                Eigen::MatrixXcd f_lmn_m = Eigen::ArrayXcd::Zero(l+1);
                double k_n = kclkk.k_ln(l, n);
#pragma omp parallel if(parallel)
                {
                    // Private storage for this thread
                    Eigen::MatrixXcd f_lmn_m_private = Eigen::ArrayXcd::Zero(l+1);
                    complex fftm_private;

                    // Computing Map and its FFT
#pragma omp for schedule(dynamic, 5)
                    for (int i = 0; i<nrings; ++i) {
                        sfb_ringdecomp_kernel(l, k_n, pix_oc, sph_bessel_l, f_lmn_m_private, &ylm_im(i, 0),
                                ring_info_container[i]);

                    }
#pragma omp critical
                    f_lmn_m += f_lmn_m_private;
                }

                kclkk.c_ln(l, n) = 2*f_lmn_m.squaredNorm()-std::pow(std::abs(f_lmn_m(0)), 2);

                // Normalize c_ln
                kclkk.c_ln(l, n) *= std::pow(norm_factor*k_n, 2)/(2*l+1);

                if (store_flmn)
                    kclkk.f_lmn.back().col(n) = f_lmn_m*k_n*norm_factor;

            }

            if (verbose) {
                std::cout << "Done." << std::endl;
            }
        }

        // Clean up
        for (auto& ring_info: ring_info_container) {
            fftw_destroy_plan(ring_info.fft_forward);
        }
        fftw_free(map);
        fftw_free(fft_map);

        // Return
        return kclkk;
    }

    inline void sfb_ringdecomp_kernel(const unsigned short& l, double& k_n,
            const PixelizedObjectContainer& pix_oc,
            const std::function<double(double)>& sbessel_fct, Eigen::MatrixXcd& f_lmn,
            const complex* y_lm_i,
            RingInfo& ring_info)
    {
        // Create ring-map
        auto current_pixelobj = ring_info.start_pixobjs;
        for (int j = 0; j<ring_info.npix; ++j) {
            ring_info.map[j] = 0;
            for (const double& r: *(current_pixelobj++)) {
                ring_info.map[j] += sbessel_fct(k_n*r);
            }
        }

        // Transform ring-map
        fftw_execute(ring_info.fft_forward);

        // Get f_lmn from transformed map, do multiply with y_lm of this ring;
        for (unsigned short m = 0; m<=l; ++m) {
            complex fft_m;

            // Fancy way of avoiding branching (if) within this loop
            /* REALITY CONDITION: fft_map[s] = fft_map[Npix-s]*
             * Real FFT: s in [0, Npix/2].
             * We get the right index by: Npix/2 - abs(Npix/2 - s)
             * In case s>Npix/2, we need also to take the complex conjugate (sign)
             */

            int m_ = ring_info.npix/2-(m%ring_info.npix);
            int sign = (m>0)-(m<0);
            m_ = ring_info.npix/2-abs(m_);

            fft_m = std::complex<double>(ring_info.fft_map[m_].real(), sign*ring_info.fft_map[m_].imag());
            f_lmn(m) += (fft_m*(*(y_lm_i++)));
        }
    }
}