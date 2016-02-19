//
// Created by Michael Bühlmann on 22/01/16.
//

#include <catana/decomposition/sfb_decomposition.hpp>
//#include <catana/tools/SphericalHarmonicsLoader.hpp>
#include <boost/math/special_functions/spherical_harmonic.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <Eigen/Dense>

#include <fftw3.h>
#include <healpix_cxx/healpix_base.h>
#include <algorithm>

#include <iostream>
#include <iomanip>

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
        if(verbose) {
            std::cout << "\tl = " << l << " ...";
            std::cout.flush();
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
                const pointing& p = pix_obj_cont[i].p;
                for (unsigned short m = 0; m<=l; ++m) {
                    y_l_mi(m, i) = std::conj(boost::math::spherical_harmonic(l, m, p.theta, p.phi));
                }
            }

            // Loop over all pixels and do Bessel decomposition:
#pragma omp for
            for (int i = 0; i<npix; ++i) {
                for (unsigned short n = 0; n<nmax; ++n) {
                    const auto& k = kclkk.k_ln(l, n);
                    for (const auto& r: pix_obj_cont[i]) {
                        f_l_in(i, n) += boost::math::sph_bessel(l, k*r);
                    }
                }
            }
        }
        // Obtain full decomposition
        f_l_mn = y_l_mi*f_l_in;

        // Norming and computing C_l(k,k)
        for (unsigned short n=0; n<nmax; ++n){
            for (int m=-l; m<=l; ++m) {
                kclkk.c_ln(l,n) += std::pow(std::abs(f_l_mn(abs(m),n)),2);
            }
            kclkk.c_ln(l,n) *= std::pow(norm_factor*kclkk.k_ln(l,n),2)/(2*l+1);
        }
        if(verbose)
            std::cout << "Done." << std::endl;
    }
    return kclkk;
}


//KClkk decomp_SFB_FFT(
//        const PixelizedObjectContainer& pix_obj_cont,
//        unsigned short lmax, unsigned short nmax,
//        double rmax, double window_volume, bool verbose, bool parallel
//) {
//    double_t norm_factor = std::sqrt(2/M_PI) * window_volume/pix_obj_cont.get_nobjects();
//    KClkk kclkk(lmax, nmax, rmax);
//
//    auto nside = pix_obj_cont.get_nside();
//    auto npix = pix_obj_cont.size();
//    auto nrings = 4*nside - 1;
//    auto hp_base = pix_obj_cont.get_hp_base();
//
//    double* map = (double*) fftw_malloc(sizeof(double)*npix);
//    complex* fft_map = (complex*) fftw_malloc(sizeof(complex)*(nrings+npix/2));
//
//
//    std::vector<fftw_plan> fftw_plans;
//    std::vector<int> ring_startpix;
//    std::vector<int> ring_npix;
//    std::vector<double> ring_phases;
//    std::vector<double> ring_theta;
//
//
//    // Prepare FFTW plans.
//    for(int i=0; i<nrings; ++i) {
//        int rstart, rpix; bool shifted;
//        hp_base.get_ring_info_small(i+1, rstart, rpix, shifted);  // Healpix ring enumeration starts at 1
//        ring_startpix.push_back(rstart);
//        ring_npix.push_back(rpix);
//        fftw_plans.push_back(fftw_plan_dft_r2c_1d(rpix, &map[rstart], (fftw_complex*) &fft_map[rstart/2+i], FFTW_MEASURE));
//        ring_phases.push_back(pix_obj_cont[rstart].p.phi);
//        ring_theta.push_back(pix_obj_cont[rstart].p.theta);
//    }
//
//    // Actual decomposition
//    for(unsigned short l=0; l<lmax; ++l){
//        for(unsigned short n=0; n<nmax; ++n){
//            double k = kclkk.k_ln(l,n);
//
//#pragma omp parallel if(parallel)
//            {
//                double c_ln_private;
//                // Computing A_sni
//#pragma omp for schedule(dynamic,5)
//                for (int i = 0; i<nrings; ++i) {
//
//                    // Compute map part (each pixel = sum over spherical bessel of contained distances)
//                    for (int j = ring_startpix[i]; j<ring_startpix[i]+ring_npix[i]; ++j) {
//                        map[j] = 0;
//                        for (const auto& r: pix_obj_cont[j]) {
//                            map[j] += boost::math::sph_bessel(l, k*r);
//                        }
//                    }
//
//                    // Compute fft_map part
//                    fftw_execute(fftw_plans[i]);
//                }
//
//                complex j(0, 1);
//#pragma omp for
//                for (unsigned short m = 0; m<=l; ++m) {
//                    complex f_lmn = 0.;
//                    for (int i = 0; i<nrings; ++i) {
//                        unsigned short m_ = (m%ring_npix[i]);
//                        complex fft_m;
//                        if (m_>ring_npix[i]/2)
//                            fft_m = std::conj(fft_map[ring_startpix[i]/2+i+ring_npix[i]-m_]);
//                        else
//                            fft_m = fft_map[ring_startpix[i]/2+i+m_];
//
//                        f_lmn += fft_m*std::conj(boost::math::spherical_harmonic(l, m, ring_theta[i], ring_phases[i]));
//                    }
//                    if (m>0)
//                        c_ln_private += 2*std::pow(std::abs(f_lmn), 2);
//                    else
//                        c_ln_private += std::pow(std::abs(f_lmn), 2);
//                }
//#pragma omp critical
//                kclkk.c_ln(l, n) += c_ln_private;
//            }  // End of parallel section
//
//            // Normalize c_ln
//            kclkk.c_ln(l,n) *= std::pow(norm_factor * k, 2) / (2*l+1);
//        }
//    }
//
//    // Clean up
//    while(fftw_plans.size()>0){
//        fftw_destroy_plan(fftw_plans.back());
//        fftw_plans.pop_back();
//    }
//    fftw_free(map);
//    fftw_free(fft_map);
//
//    // Return
//    return kclkk;
//}
//
//
//
//KClkk decomp_SFB_FFT_v2(
//        const PixelizedObjectContainer& pix_obj_cont,
//        unsigned short lmax, unsigned short nmax,
//        double rmax, double window_volume, bool verbose, bool parallel
//) {
//    double_t norm_factor = std::sqrt(2/M_PI) * window_volume/pix_obj_cont.get_nobjects();
//    KClkk kclkk(lmax, nmax, rmax);
//
//    auto nside = pix_obj_cont.get_nside();
//    auto npix = pix_obj_cont.size();
//    auto nrings = 4*nside - 1;
//    auto hp_base = pix_obj_cont.get_hp_base();
//
//    double* map = (double*) fftw_malloc(sizeof(double)*npix);
//    complex* fft_map = (complex*) fftw_malloc(sizeof(complex)*(nrings+npix/2));
//
//    std::vector<fftw_plan> fftw_plans;
//    std::vector<int> ring_startpix;
//    std::vector<int> ring_npix;
//    std::vector<double> ring_phases;
//    std::vector<double> ring_theta;
//
//
//    // Prepare FFTW plans.
//    for(int i=0; i<nrings; ++i) {
//        int rstart, rpix; bool shifted;
//        hp_base.get_ring_info_small(i+1, rstart, rpix, shifted);  // Healpix ring enumeration starts at 1
//        ring_startpix.push_back(rstart);
//        ring_npix.push_back(rpix);
//        fftw_plans.push_back(fftw_plan_dft_r2c_1d(rpix, &map[rstart], (fftw_complex*) &fft_map[rstart/2+i], FFTW_MEASURE));
//        ring_phases.push_back(pix_obj_cont[rstart].p.phi);
//        ring_theta.push_back(pix_obj_cont[rstart].p.theta);
//    }
//
//    // Actual decomposition
//    for(unsigned short l=0; l<lmax; ++l){
//
//        // Precompute spherical harmonics
//        Eigen::ArrayXXcd ylm_im(nrings, l+1);
//
//#pragma omp parallel for if(parallel) schedule(dynamic, 5)
//        for(int i=0; i<nrings; ++i){
//            for(unsigned short m=0; m<=l; ++m){
//                ylm_im(i,m) = std::conj(boost::math::spherical_harmonic(l, m, ring_theta[i], ring_phases[i]));
//            }
//        }
//
//
//        for(unsigned short n=0; n<nmax; ++n){
//            double k_n = kclkk.k_ln(l,n);
//#pragma omp parallel if(parallel)
//            {
//                double c_ln_private = 0;
//                // Computing Map and its FFT
//#pragma omp for schedule(dynamic, 5)
//                for (int i = 0; i<nrings; ++i) {
//                    for (int j = ring_startpix[i]; j<ring_startpix[i]+ring_npix[i]; ++j) {
//                        map[j] = 0;
//                        for (const auto& r: pix_obj_cont[j]) {
//                            map[j] += boost::math::sph_bessel(l, k_n*r);
//                        }
//                    }
//                    fftw_execute(fftw_plans[i]);
//                }
//
//#pragma omp for
//                for (unsigned short m = 0; m<=l; ++m) {
//                    complex f_lmn = 0.;
//                    for (int i = 0; i<nrings; ++i) {
//                        unsigned short m_ = (m%ring_npix[i]);
//                        complex fft_m;
//                        if (m_>ring_npix[i]/2)
//                            fft_m = std::conj(fft_map[ring_startpix[i]/2+i+ring_npix[i]-m_]);
//                        else
//                            fft_m = fft_map[ring_startpix[i]/2+i+m_];
//                        f_lmn += fft_m*ylm_im(i, m);
//                    }
//
//                    if (m>0)
//                        c_ln_private += 2*std::pow(std::abs(f_lmn), 2);
//                    else
//                        c_ln_private += std::pow(std::abs(f_lmn), 2);
//                }
//#pragma omp critical
//                kclkk.c_ln(l,n) += c_ln_private;
//            }  // End of parallel section
//
//            // Normalize c_ln
//            kclkk.c_ln(l,n) *= std::pow(norm_factor * k_n, 2) / (2*l+1);
//        }
//    }
//
//    // Clean up
//    while(fftw_plans.size()>0){
//        fftw_destroy_plan(fftw_plans.back());
//        fftw_plans.pop_back();
//    }
//    fftw_free(map);
//    fftw_free(fft_map);
//
//    // Return
//    return kclkk;
//}

KClkk decomp_SFB_FFT(
        const PixelizedObjectContainer& pix_obj_cont,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool verbose, bool parallel
) {
    double_t norm_factor = std::sqrt(2/M_PI) * window_volume/pix_obj_cont.get_nobjects();
    KClkk kclkk(lmax, nmax, rmax);

    auto nside = pix_obj_cont.get_nside();
    auto npix = pix_obj_cont.size();
    auto nrings = 4*nside - 1;
    auto hp_base = pix_obj_cont.get_hp_base();

    double* map = (double*) fftw_malloc(sizeof(double)*npix);
    complex* fft_map = (complex*) fftw_malloc(sizeof(complex)*(nrings+npix/2));

    std::vector<fftw_plan> fftw_plans;
    std::vector<int> ring_startpix;
    std::vector<int> ring_npix;
    std::vector<double> ring_phases;
    std::vector<double> ring_theta;


    // Prepare FFTW plans.
    for(int i=0; i<nrings; ++i) {
        int rstart, rpix; bool shifted;
        hp_base.get_ring_info_small(i+1, rstart, rpix, shifted);  // Healpix ring enumeration starts at 1
        ring_startpix.push_back(rstart);
        ring_npix.push_back(rpix);
        fftw_plans.push_back(fftw_plan_dft_r2c_1d(rpix, &map[rstart], (fftw_complex*) &fft_map[rstart/2+i], FFTW_MEASURE));
        ring_phases.push_back(pix_obj_cont[rstart].p.phi);
        ring_theta.push_back(pix_obj_cont[rstart].p.theta);
    }

    // Actual decomposition
    for(unsigned short l=0; l<lmax; ++l){

        // Precompute spherical harmonics
        Eigen::ArrayXXcd ylm_im(nrings, l+1);
#pragma omp parallel for if(parallel) schedule(dynamic, 5)
        for(int i=0; i<nrings; ++i){
            for(unsigned short m=0; m<=l; ++m){
                ylm_im(i,m) = std::conj(boost::math::spherical_harmonic(l, m, ring_theta[i], ring_phases[i]));
            }
        }

        for(unsigned short n=0; n<nmax; ++n){
            Eigen::MatrixXcd f_lmn_m = Eigen::ArrayXcd::Zero(l+1);
            double k_n = kclkk.k_ln(l,n);
#pragma omp parallel if(parallel)
            {
                Eigen::MatrixXcd f_lmn_m_private = Eigen::ArrayXcd::Zero(l+1);

                // Computing Map and its FFT
#pragma omp for schedule(dynamic, 5)
                for (int i = 0; i<nrings; ++i) {
                    for (int j = ring_startpix[i]; j<ring_startpix[i]+ring_npix[i]; ++j) {
                        map[j] = 0;
                        for (const auto& r: pix_obj_cont[j]) {
                            map[j] += boost::math::sph_bessel(l, k_n*r);
                        }
                    }
                    fftw_execute(fftw_plans[i]);

                    for (unsigned short m = 0; m<=l; ++m) {
                        complex fft_m;
                        unsigned short m_ = (m%ring_npix[i]);

                        if (m_>ring_npix[i]/2)
                            fft_m = std::conj(fft_map[ring_startpix[i]/2+i+ring_npix[i]-m_]);
                        else
                            fft_m = fft_map[ring_startpix[i]/2+i+m_];

                        f_lmn_m_private(m) += (fft_m*ylm_im(i, m));
                    }

                }
#pragma omp critical
                f_lmn_m += f_lmn_m_private;
            }

            kclkk.c_ln(l,n) = 2*f_lmn_m.squaredNorm() - std::pow(std::abs(f_lmn_m(0)),2);

            // Normalize c_ln
            kclkk.c_ln(l,n) *= std::pow(norm_factor * k_n, 2) / (2*l+1);
        }
    }

    // Clean up
    while(fftw_plans.size()>0){
        fftw_destroy_plan(fftw_plans.back());
        fftw_plans.pop_back();
    }
    fftw_free(map);
    fftw_free(fft_map);

    // Return
    return kclkk;
}