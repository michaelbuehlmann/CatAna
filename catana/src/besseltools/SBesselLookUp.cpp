//
// Created by Michael Bühlmann on 19/02/16.
//

#include <catana/besseltools/SBesselLookUp.hpp>
#include <catana/besseltools/BesselZeros.hpp>
#include <boost/math/special_functions/bessel.hpp>

namespace catana { namespace besseltools {

        SBesselLookUp::SBesselLookUp(unsigned short l, unsigned short nmax, unsigned int n_interp)
                :l(l), n_interp(n_interp)
        {
            SphericalBesselZeros bz(l);
            z_max = bz[nmax];
            delta_z_inv = (n_interp-1)/z_max;
            lut.reset(new double[n_interp]);
            load_lut();
        }

        double SBesselLookUp::operator()(const double& z) const
        {
            assert(z<z_max);
            double i_approx = z*delta_z_inv;
            int i = static_cast<int>(i_approx);
            return lut[i]+(lut[i+1]-lut[i])*(i_approx-i);
        }

        void SBesselLookUp::load_lut()
        {
            double delta_z = 1./delta_z_inv;

#pragma omp parallel for
            for (unsigned int i = 0; i<n_interp; ++i) {
                lut[i] = boost::math::sph_bessel(l, delta_z*i);
            }
        }

        double SBesselLookUp::get_zmax() const
        {
            return z_max;
        }

}}