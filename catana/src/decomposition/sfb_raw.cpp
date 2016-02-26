//
// Created by Michael Bühlmann on 23/02/16.
//

#include <catana/decomposition/sfb_raw.hpp>
#include <catana/besseltools.hpp>
#include <catana/config.hpp>

#include <boost/math/special_functions.hpp>



using complex = std::complex<double_t>;

KClkk _sfb_raw(
        const ObjectContainer& objects,
        unsigned short lmax, unsigned short nmax,
        double rmax, double window_volume, bool store_flmn, bool verbose, bool parallel, bool interpolated)
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

    // This will be a nullptr if !interpolated, otherwise we will assign it within the l-for loop
    std::unique_ptr<SBesselLookUp> sblu;
    std::function <double(double)> sph_bessel_l;

    // Actual decomposition
    for(unsigned short l=0; l<lmax; ++l){

        if(verbose) {
            std::cout << "\tl = " << l << " ...";
            std::cout.flush();
        }

        if(interpolated){
            sblu.reset(new SBesselLookUp(l, nmax, BESSELINTERPOLATIONPOINTS_PER_ZERO*nmax));
            sph_bessel_l = [&](const double& z){return sblu->operator()(z);};
        } else {
            sph_bessel_l = [&](const double& z){return boost::math::sph_bessel(l, z);};
        }

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
                    j_l_n(n) = sph_bessel_l(kclkk.k_ln(l, n)*obj_it->r);
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

        if(store_flmn)
            kclkk.f_lmn.push_back(f_l_mn);

        if(verbose)
            std::cout << "Done.\n";
    }

    return kclkk;
}
