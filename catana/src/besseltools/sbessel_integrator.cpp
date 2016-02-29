//
// Created by Michael Bühlmann on 19/02/16.
//

#include <catana/besseltools/sbessel_integrator.hpp>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>
#include <catana/tools/gsl_function_wrapper.hpp>
#include <boost/math/special_functions/bessel.hpp>

#include <iostream>

namespace catana { namespace besseltools {

    // Machine epsilon of double
    typedef std::numeric_limits<double_t> double_t_lim;

    double_t double_sbessel_integrator(std::function<double_t(double_t)> f, const unsigned int& l,
            const double_t& Rmax,
            double_t k1, double_t k2)
    {
        SphericalBesselZeros bz(l);
        return double_sbessel_integrator_bz(f, l, bz, Rmax, k1, k2);
    }

    double_t double_sbessel_integrator_bz(std::function<double_t(double_t)> f, const unsigned int& l,
            SphericalBesselZeros& bz,
            const double_t& Rmax,
            double_t k1, double_t k2)
    {
        gsl_set_error_handler_off();

        auto integrand = [&](double_t r) {
            return f(r)*boost::math::sph_bessel(l, k1*r)*boost::math::sph_bessel(l, k2*r);
        };
        if (k1>k2) {
            std::swap(k1, k2);
        }

        gsl_function_pp<decltype(integrand)> Fp(integrand);
        gsl_function* F = static_cast<gsl_function*>(&Fp);

        auto workspace = gsl_integration_workspace_alloc(1000);
        double_t result = 0.;
        double_t result_old, result_intermediate, temp, error;
        int exit_code;

        double_t lower_z = 0;
        double_t upper_z, upper_z_large;

        unsigned int i = 0;
        unsigned int j = 0;

        do {
            upper_z_large = std::min(bz[i]/k1, Rmax);
            result_intermediate = 0.;
            do {
                upper_z = (bz[j]/k2>upper_z_large) ? upper_z_large : bz[j++]/k2;
//                std::cout << "\t " << lower_z << ",\t" << upper_z << ": ";
                exit_code = gsl_integration_qag(F, lower_z, upper_z, 0, 100*double_t_lim::epsilon(), 1000, 2,
                        workspace, &temp, &error);
                if (exit_code==GSL_EROUND) {
                    std::cout << "round-off error for integration between "
                            << lower_z << ", " << upper_z << std::endl;
                } else {
                    result_intermediate += temp;
                }
                lower_z = upper_z;
            } while (upper_z<upper_z_large);

            result_old = result;
            result += result_intermediate;
            ++i;
        } while (result_old!=result && upper_z<Rmax);

        gsl_integration_workspace_free(workspace);
        return result;
    }

    SBesselTransformedFunction::SBesselTransformedFunction(std::function<double_t(double_t)> f, unsigned int l,
            double_t Rmax)
            :function(f), l(l), bz(l), Rmax(Rmax) { }

    double_t SBesselTransformedFunction::operator()(const double_t& k1, const double_t& k2)
    {
        auto fct = [&](double_t r) {
            return 2./M_PI*std::pow(r, 2)*function(r);
        };
        return double_sbessel_integrator_bz(fct, l, bz, Rmax, k1, k2);
    }
}}