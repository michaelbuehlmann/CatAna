//
// Created by Michael Bühlmann on 19/02/16.
//

#include <catana/besseltools/SBesselInterpolator.hpp>
#include <catana/besseltools/BesselZeros.hpp>
#include <boost/math/special_functions/bessel.hpp>

namespace catana { namespace besseltools {

    SBesselInterpolator SBesselInterpolator_uptozero(unsigned short l, unsigned short nmax,
            unsigned int interpolation_points) {
        return SBesselInterpolator(l, SphericalBesselZeros(l)[nmax], interpolation_points);
    }

    SBesselInterpolator::SBesselInterpolator(unsigned short l, double z_max, unsigned int interpolation_points)
            :l(l), FunctionInterpolator([l](double z) { return boost::math::sph_bessel(l, z); }, interpolation_points,
            0, z_max) { }

    double SBesselInterpolator::operator()(const double& z) const
    {
        assert(z<x_max);
        double i_approx = z*dx_inv;
        int i = static_cast<int>(i_approx);
        return y_values[i]+(y_values[i+1]-y_values[i])*(i_approx-i);
    }

    double SBesselInterpolator::get_zmax() const
    {
        return x_max;
    }

    SBesselIFunction::SBesselIFunction(double z_max, unsigned int interpolation_points)
        :z_max(z_max), interpolation_points(interpolation_points) { }

    void SBesselIFunction::initialize(unsigned int l)
    {
        if (l>interpolators.size()+1)
            interpolators.resize(l+1);
        if (!interpolators[l].second)
            interpolators[l].first.reset(new SBesselInterpolator(l, z_max, interpolation_points));
    }

    void SBesselIFunction::deallocate(unsigned int l)
    {
        assert(l<=interpolators.size()+1);
        interpolators[l].first.reset();
        interpolators[l].second = false;
    }

    double SBesselIFunction::operator()(unsigned int l, double z)
    {
        assert(l<=interpolators.size()+1);
        assert(interpolators[l].second);

        return interpolators[l].first->operator()(z);
    }

}}