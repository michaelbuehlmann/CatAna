//
// Created by Michael Bühlmann on 19/02/16.
//

#include <catana/besseltools/SBesselInterpolator.hpp>
#include <catana/besseltools/BesselZeros.hpp>
#include <boost/math/special_functions/bessel.hpp>

namespace catana { namespace besseltools {

        SBesselInterpolator::SBesselInterpolator(unsigned short l, unsigned short nmax, unsigned int interpolation_points)
                :l(l), FunctionInterpolator( [l](double z){return boost::math::sph_bessel(l, z);}, interpolation_points, 0, SphericalBesselZeros(l)[nmax])
        {}

        double SBesselInterpolator::operator()(const double& z) const
        {
            assert(z<x_max);
            double i_approx = z * dx_inv;
            int i = static_cast<int>(i_approx);
            return y_values[i]+(y_values[i+1]-y_values[i])*(i_approx-i);
        }

        double SBesselInterpolator::get_zmax() const
        {
            return x_max;
        }

}}