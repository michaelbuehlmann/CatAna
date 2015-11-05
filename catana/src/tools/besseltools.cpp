//
// Created by Michael Bühlmann on 01/11/15.
//

#include "catana/tools/besseltools.hpp"
#include <utility>
#include <boost/math/special_functions/bessel.hpp>
#include <iostream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>


typedef std::numeric_limits< double_t > double_t_lim;


BesselZeros::BesselZeros(double_t l)
    : l(l)
{
    zeros.push_back(first_zero());
    zeros.push_back(second_zero());
}

double_t BesselZeros::first_zero() {
    double_t z;
    if(l<1) {
        z = 2.404826;
    } else {
        z = l + 1.8557571* std::pow(l,1./3.)
              + 1.033150 * std::pow(l,-1./3.)
              - 0.00397  * std::pow(l,-1.)
              - 0.0908   * std::pow(l,-5./3.)
              + 0.043    * std::pow(l,-7./3.);
    }
    z = newton_iterate(z);
    return z;
}

double_t BesselZeros::second_zero() {
    double_t z;
    if(l<1) {
        z = 5.520078;
    } else {
        z = l + 3.2446076* std::pow(l,1./3.)
              + 3.158244 * std::pow(l,-1./3.)
              - 0.08331  * std::pow(l,-1.)
              - 0.8437   * std::pow(l,-5./3.)
              + 0.864    * std::pow(l,-7./3.);
    }
    z = newton_iterate(z);
    return z;
}

double_t BesselZeros::newton_iterate(double_t z) {
    double_t z_old;
    double_t delta;
    double_t jl;

    // Some debugging...
//    unsigned int counter = 0;

    do {
        jl = boost::math::cyl_bessel_j(l,z);
        delta = jl/(l/z*jl - boost::math::cyl_bessel_j(l+1,z));

        z_old = z;
        z -= delta;
//        ++counter;
    } while (z_old != z);

//    std::cout << "Newton needed " << counter << " iterations" << std::endl;
    return z;
}

double_t BesselZeros::next() {
    double_t z = 2 * zeros.rbegin()[0] - zeros.rbegin()[1];
    z = newton_iterate(z);
    return z;
}

void BesselZeros::compute_up_to(double_t z_max) {
    while(zeros.back() < z_max){
        zeros.push_back(next());
    }
}

double_t BesselZeros::operator[](unsigned int n) {
    while (zeros.size() <= n) {
        zeros.push_back(next());
    }
    return zeros[n];
}

// Some wrapper for function -> gsl_function pointers
template< typename F >  class gsl_function_pp : public gsl_function {
public:
    gsl_function_pp(const F& func) : _func(func) {
        function = &gsl_function_pp::invoke;
        params=this;
    }
private:
    const F& _func;
    static double invoke(double x, void *params) {
        return static_cast<gsl_function_pp*>(params)->_func(x);
    }
};


double_t double_sbessel_integrator(std::function<double_t(double_t)> f, unsigned int l, double_t Rmax, double_t k1, double_t k2) {
    gsl_set_error_handler_off();

    auto integrand = [&](double_t r){
        return f(r)*boost::math::sph_bessel(l,k1*r)*boost::math::sph_bessel(l,k2*r);
    };
    if (k1 > k2) {
        std::swap(k1, k2);
    }

    gsl_function_pp<decltype(integrand)> Fp(integrand);
    gsl_function *F = static_cast<gsl_function*>(&Fp);

    auto workspace = gsl_integration_workspace_alloc(1000);
    double_t result = 0.;
    double_t result_old, result_intermediate, temp, error;
    int exit_code;

    double_t lower_z = 0;
    double_t upper_z, upper_z_large;

    // Compute all Bessel Zeros we will need
    BesselZeros bz(l+0.5);

    unsigned int i=0;
    unsigned int j=0;

    do {
        upper_z_large = std::min(bz[i] / k1, Rmax);
        result_intermediate = 0.;
        do {
            upper_z = (bz[j]/k2 > upper_z_large) ? upper_z_large : bz[j++]/k2;
//                std::cout << "\t " << lower_z << ",\t" << upper_z << ": ";
            exit_code = gsl_integration_qag(F, lower_z, upper_z, 0, 100 * double_t_lim::epsilon(), 1000, 2, workspace, &temp,
                                &error);
            if(exit_code==GSL_EROUND){
                std::cout << "round-off error for integration between "
                << lower_z << ", " << upper_z << std::endl;
            } else {
//                    std::cout << temp << " += " << error << std::endl;
                result_intermediate += temp;
            }
            lower_z = upper_z;
        } while(upper_z < upper_z_large);

        result_old = result;
        result += result_intermediate;
        ++i;
    } while (result_old != result && upper_z < Rmax);

    return result;
}


