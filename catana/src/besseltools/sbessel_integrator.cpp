#include <catana/besseltools/sbessel_integrator.hpp>
#include <catana/tools/gsl_function_wrapper.hpp>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_errno.h>

#include <limits>
#include <iostream>
#include <cmath>


namespace catana { namespace besseltools {

  // Machine epsilon of double
  typedef std::numeric_limits<double> double_lim;

  template<class T>
  double SimpsonIntegration(T f, double a, double b, int N) {
    /*
    Doing Simpson integration on function f with N boxes
    Input:
        f:	function which takes 1 value and returns 1
        a:	lower bound of integration
        b:	upper bound of integration
        N:	Number of boxes in which interval [a,b] is divides

    Output:
        Value of integration of f between a and b
    */

    double h = 0.5 * (b - a) / N; //Size of half a step
    double s = f(a) + f(b); //initializing sum with boundary values

    for(int i = 0; i < N; ++i) //summing up all half-step values
    {
      s += 4 * f(a + h * (2 * i + 1));
    }
    for(int i = 1; i < N; ++i) //summing up all full-step values
    {
      s += 2 * f(a + h * (2 * i));
    }

    return s * h / 3;
  }

  double double_sbessel_integrator(std::function<double(double)> f, const unsigned int& l,
                                   const double& Rmax,
                                   double k1, double k2) {
    SphericalBesselZeros bz(l);
    return double_sbessel_integrator_bz(f, l, bz, Rmax, k1, k2);
  }


  double double_sbessel_integrator_bz(std::function<double(double)> f, const unsigned int& l,
                                      SphericalBesselZeros& bz,
                                      const double& Rmax,
                                      double k1, double k2) {
    gsl_set_error_handler_off();

    auto integrand = [&](double r) {
      return f(r) * gsl_sf_bessel_jl(l, k1 * r) * gsl_sf_bessel_jl(l, k2 * r);
    };
    if(k1 > k2) {
      std::swap(k1, k2);
    }

    gsl_function_pp<decltype(integrand)> Fp(integrand);
    gsl_function *F = static_cast<gsl_function *>(&Fp);

    auto workspace = gsl_integration_workspace_alloc(1000);
    double result = 0.;
    double result_old, result_intermediate, temp, error;
    int exit_code;

    SBesselIntegrationRangeGenerator sbirg_l(bz, k1);
    sbirg_l.set_boundaries(0, Rmax);
    SBesselIntegrationRangeGenerator sbirg_s(bz, k2);

    auto range_l = sbirg_l.next();


    while(range_l.first != range_l.second) {
      sbirg_s.set_boundaries(range_l.first, range_l.second);
      result_intermediate = 0;
      auto range_s = sbirg_s.next();
      while(range_s.first != range_s.second) {
//                std::cout << "\t\t(" << range_s.first << " " << range_s.second << ")" << std::endl;
        exit_code = gsl_integration_qag(F, range_s.first, range_s.second, 0, 1e-7, 1000, 2,
                                        workspace, &temp, &error);
//                if (exit_code==GSL_EROUND) {
//                    std::cout << "round-off error for integration between "
//                            << range_s.first << ", " << range_s.second << std::endl;
//                } else {
        result_intermediate += temp;
//                }

        range_s = sbirg_s.next();
      }

//            std::cout << "\t(" << range_l.first << " " << range_l.second << "): \t" << result_intermediate << std::endl;
      result_old = result;
      result += result_intermediate;
      if(std::abs(result - result_old) < result * 1e-10)
        break;

      range_l = sbirg_l.next();
    }

    gsl_integration_workspace_free(workspace);
    return result;
  }


  SBesselIntegrationRangeGenerator::SBesselIntegrationRangeGenerator(SphericalBesselZeros& bz, double k)
      : k(k), bz(bz) {}

  void SBesselIntegrationRangeGenerator::set_boundaries(double r_min, double r_max) {
    this->r_max = r_max;
    this->current_low = r_min;
    unsigned int ix = bz.next_zero(r_min * k);

    current_high_idx = (bz[ix] / k > r_min) ? ix : ix + 1;
//        std::cout << "Set boundaries called: [" << r_min << ", " << r_max << "], set low=" << current_low
//                << ", high_idx=" << current_high_idx << " (" << bz[current_high_idx]/k << ")" << std::endl;
  }

  std::pair<double, double> SBesselIntegrationRangeGenerator::next() {
    double low = current_low;
    double high = std::min(bz[current_high_idx] / k, r_max);

    current_low = high;
    current_high_idx++;

    return std::make_pair(low, high);
  }
}}