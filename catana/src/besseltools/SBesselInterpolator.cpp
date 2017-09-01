#include <catana/besseltools/SBesselInterpolator.hpp>
#include <catana/besseltools/SphericalBesselZeros.hpp>

#include <gsl/gsl_sf_bessel.h>
#include <cassert>


namespace catana { namespace besseltools {

  SBesselInterpolator SBesselInterpolator_uptozero(unsigned short l, unsigned short nmax,
                                                   unsigned int interpolation_points) {
    return SBesselInterpolator(l, SphericalBesselZeros(l)[nmax], interpolation_points);
  }

  SBesselInterpolator::SBesselInterpolator(unsigned short l, double z_max, unsigned int interpolation_points)
      : FunctionInterpolator([l](double z) { return gsl_sf_bessel_jl(l, z); }, interpolation_points,
                             0, z_max), l(l) {}

  double SBesselInterpolator::operator()(const double& z) const {
    if(z >= x_max) {
      throw std::domain_error("argument (z) outside of defined range.");
    }
    double i_approx = z * dx_inv;
    int i = static_cast<int>(i_approx);
    return y_values[i] + (y_values[i + 1] - y_values[i]) * (i_approx - i);
  }

  double SBesselInterpolator::get_zmax() const {
    return x_max;
  }

  SBesselIFunction::SBesselIFunction(double z_max, unsigned int interpolation_points)
      : z_max(z_max), interpolation_points(interpolation_points) {}

  void SBesselIFunction::initialize(unsigned int l) {
    if(l > interpolators.size() + 1)
      interpolators.resize(l + 1);
    if(!interpolators[l].second)
      interpolators[l].first.reset(new SBesselInterpolator(l, z_max, interpolation_points));
  }

  void SBesselIFunction::deallocate(unsigned int l) {
    assert(l <= interpolators.size() + 1);
    interpolators[l].first.reset();
    interpolators[l].second = false;
  }

  double SBesselIFunction::operator()(unsigned int l, double z) {
    if(l > interpolators.size() + 1) {
      throw std::domain_error("argument (l) too large for this instance of SBesselInterpolator");
    }
    if(!interpolators[l].second) {
      throw std::logic_error("interpolator for requested l not initialized");
    }
    return interpolators[l].first->operator()(z);
  }

}}