#include <catana/besseltools/SphericalBesselZeros.hpp>
#include <iostream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <gsl/gsl_sf_bessel.h>


namespace catana { namespace besseltools {

  SphericalBesselZeros::SphericalBesselZeros(unsigned int l)
      : l(l) {
    zeros.push_back(first_zero());
    zeros.push_back(second_zero());
  }

  double SphericalBesselZeros::first_zero() {
    double z;
    if(l == 0) {
      z = 3.141592653589793238463;
    } else {
      z = l + 0.5
          + 1.8557571 * std::pow(l + 0.5, 1. / 3.)
          + 1.033150 * std::pow(l + 0.5, -1. / 3.)
          - 0.00397 * std::pow(l + 0.5, -1.)
          - 0.0908 * std::pow(l + 0.5, -5. / 3.)
          + 0.043 * std::pow(l + 0.5, -7. / 3.);
    }
    z = newton_iterate(z);
    return z;
  }

  double SphericalBesselZeros::second_zero() {
    double z;
    if(l == 0) {
      z = 6.283185307179586476925;
    } else {
      z = l + 0.5
          + 3.2446076 * std::pow(l + 0.5, 1. / 3.)
          + 3.158244 * std::pow(l + 0.5, -1. / 3.)
          - 0.08331 * std::pow(l + 0.5, -1.)
          - 0.8437 * std::pow(l + 0.5, -5. / 3.)
          + 0.864 * std::pow(l + 0.5, -7. / 3.);
    }
    z = newton_iterate(z);
    return z;
  }

  double SphericalBesselZeros::newton_iterate(double z) {
    double delta;
    double jl;

    do {
      jl = gsl_sf_bessel_jl(l, z);
      delta = jl / (l / z * jl - gsl_sf_bessel_jl(l + 1, z));

      z -= delta;

    } while(delta / z > 1e-15);

    return z;
  }

  double SphericalBesselZeros::next() {
    double z = 2 * zeros.rbegin()[0] - zeros.rbegin()[1];
    z = newton_iterate(z);
    return z;
  }

  void SphericalBesselZeros::compute_up_to(double z_max) {
    while(zeros.back() < z_max) {
      zeros.push_back(next());
    }
  }

  unsigned int SphericalBesselZeros::next_zero(double z) {
    compute_up_to(z);
    auto index_it = std::find_if(zeros.begin(), zeros.end(), [&](double& _z) { return _z > z; });
    return std::distance(zeros.begin(), index_it);
  }

//    std::pair<unsigned int, unsigned int> SphericalBesselZeros::within(double z_min, double z_max)
//    {
//        compute_up_to(z_max);
//        auto first = std::find_if(zeros.begin(), zeros.end(), [&](double& z){return z>z_min;});
//        auto last = std::find_if(first, zeros.end(), [&](double& z){return z>z_max;})--;
//
//
//        return std::make_pair(
//                std::distance(zeros.begin(), first),
//                std::distance(zeros.begin(), last)
//        );
//    }

  double SphericalBesselZeros::operator[](unsigned int n) {
    while(zeros.size() <= n) {
      zeros.push_back(next());
    }
    return zeros[n];
  }

}}