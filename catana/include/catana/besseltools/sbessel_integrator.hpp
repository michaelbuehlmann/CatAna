#ifndef CATANA_SBESSEL_INTEGRATOR_HPP
#define CATANA_SBESSEL_INTEGRATOR_HPP

#include <functional>
#include <memory>
#include "../tools/FunctionInterpolator.hpp"
#include "SphericalBesselZeros.hpp"


namespace catana { namespace besseltools {

  //! Computes the integral over f(r)*j_l(k1*r)*j_l(k2*r) between [0,Rmax]
  double double_sbessel_integrator(
      std::function<double(double)> f,
      const unsigned int& l,
      const double& Rmax,
      double k1, double k2);


  //! Computes the integral over f(r)*j_l(k1*r)*j_l(k2*r) between [0,Rmax]
  /*
   * The order of bz must be of the same l!
   */
  double double_sbessel_integrator_bz(
      std::function<double(double)> f,
      const unsigned int& l,
      SphericalBesselZeros& bz,
      const double& Rmax,
      double k1, double k2);


  class SBesselIntegrationRangeGenerator {
  public:
    SBesselIntegrationRangeGenerator(SphericalBesselZeros& bz, double k);

    void set_boundaries(double r_min, double r_max);

    std::pair<double, double> next();

  private:
    double k;
    SphericalBesselZeros& bz;
    double r_max;
    double current_low;
    int current_high_idx;
  };

}}
#endif //CATANA_SBESSEL_INTEGRATOR_HPP
