#include <catana/decomposition/sfb_raw.hpp>
#include <catana/besseltools/SBesselInterpolator.hpp>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_sf_legendre.h>


namespace catana {

  using complex = std::complex<double_t>;

  KClkk _sfb_raw(const PointContainer& points, unsigned short lmax, unsigned short nmax, double rmax, bool store_flmn,
                   bool verbose, bool parallel, bool interpolated) {
    size_t container_size = size_t(std::distance(std::begin(points), std::end(points)));
    if(verbose) {
      std::cout << "SFB decomposition (raw method):"
                << " lmax=" << lmax
                << " nmax=" << nmax
                << " Rmax=" << rmax
                << std::endl;
      std::cout << "Catalog consists of " << container_size << " points." << std::endl;
    }

    // Turn off GSL Error handler
    auto gsl_error_handler_old = gsl_set_error_handler_off();

    // Compute normalization factor and set up KClkk
    double norm_factor = std::sqrt(2. / M_PI) * (4./3.) * M_PI;
    norm_factor *= rmax*rmax*rmax / container_size;
    KClkk kclkk(lmax, nmax, rmax);

    // This will be a nullptr if !interpolated, otherwise we will assign it within the l-for loop
    std::unique_ptr<besseltools::SBesselInterpolator> sblu;
    std::function<double(double)> sph_bessel_l;

    // Actual decomposition
    for(unsigned short l = 0; l < lmax; ++l) {

      if(verbose) {
        std::cout << "\tl = " << l << " ...";
        std::cout.flush();
      }

      if(interpolated) {
        sblu.reset(new besseltools::SBesselInterpolator(
            besseltools::SBesselInterpolator_uptozero(l, nmax, BESSELINTERPOLATIONPOINTS_PER_ZERO * nmax)
        ));
        sph_bessel_l = [&](const double& z) { return sblu->operator()(z); };
      } else {
        sph_bessel_l = [&](const double& z) { return gsl_sf_bessel_jl(l, z); };
      }

      Eigen::ArrayXXcd f_l_mn = Eigen::ArrayXXcd::Zero(l + 1, nmax);

#pragma omp parallel if(parallel)
      {
        Eigen::ArrayXXcd f_l_mn_private = Eigen::ArrayXXcd::Zero(l + 1, nmax);

        // Loop over all points
#pragma omp for nowait
        for(PointContainer::const_iterator obj_it = points.begin(); obj_it < points.end(); ++obj_it) {

          // Compute j_ln for all n
          Eigen::ArrayXd j_l_n(nmax);

          for(unsigned short n = 0; n < nmax; ++n) {
            j_l_n(n) = sph_bessel_l(kclkk.k_ln(l, n) * obj_it->r);
          }

          // Compute Y_lm
          for(unsigned short m = 0; m <= l; ++m) {
            double gsl_sphPlm = gsl_sf_legendre_sphPlm(l, m, std::cos(obj_it->p.theta));
            complex y_lm(gsl_sphPlm * std::cos(-m * obj_it->p.phi), gsl_sphPlm * std::sin(-m * obj_it->p.phi));

            for(unsigned short n = 0; n < nmax; ++n) {
              f_l_mn_private(m, n) += kclkk.k_ln(l, n) * j_l_n(n) * y_lm;
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

      for(unsigned short n = 0; n < nmax; ++n) {
        for(int m = -l; m <= l; ++m) {
          kclkk.c_ln(l, n) += std::pow(std::abs(f_l_mn(abs(m), n)), 2);
        }
        kclkk.c_ln(l, n) /= (2 * l + 1);
      }

      if(store_flmn)
        kclkk.f_lmn.push_back(f_l_mn);

      if(verbose)
        std::cout << "Done.\n";
    }

    // Reset old GSL error handler
    gsl_set_error_handler(gsl_error_handler_old);
    return kclkk;
  }
}
