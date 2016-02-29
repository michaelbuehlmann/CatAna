//
// Created by Michael Bühlmann on 01/11/15.
//

#include <catana/besseltools/BesselZeros.hpp>
#include <utility>
#include <boost/math/special_functions/bessel.hpp>
#include <iostream>

namespace catana { namespace besseltools {

    BesselZeros::BesselZeros(double_t l)
            :l(l)
    {
        zeros.push_back(first_zero());
        zeros.push_back(second_zero());
    }

    double_t BesselZeros::first_zero()
    {
        double_t z;
        if (l<1) {
            z = 2.404826;
        } else {
            z = l+1.8557571*std::pow(l, 1./3.)
                    +1.033150*std::pow(l, -1./3.)
                    -0.00397*std::pow(l, -1.)
                    -0.0908*std::pow(l, -5./3.)
                    +0.043*std::pow(l, -7./3.);
        }
        z = newton_iterate(z);
        return z;
    }

    double_t BesselZeros::second_zero()
    {
        double_t z;
        if (l<1) {
            z = 5.520078;
        } else {
            z = l+3.2446076*std::pow(l, 1./3.)
                    +3.158244*std::pow(l, -1./3.)
                    -0.08331*std::pow(l, -1.)
                    -0.8437*std::pow(l, -5./3.)
                    +0.864*std::pow(l, -7./3.);
        }
        z = newton_iterate(z);
        return z;
    }

    double_t BesselZeros::newton_iterate(double_t z)
    {
        double_t z_old;
        double_t delta;
        double_t jl;

        do {
            jl = boost::math::cyl_bessel_j(l, z);
            delta = jl/(l/z*jl-boost::math::cyl_bessel_j(l+1, z));

            z_old = z;
            z -= delta;
        }
        while (z_old!=z);

        return z;
    }

    double_t BesselZeros::next()
    {
        double_t z = 2*zeros.rbegin()[0]-zeros.rbegin()[1];
        z = newton_iterate(z);
        return z;
    }

    void BesselZeros::compute_up_to(double_t z_max)
    {
        bool added(false);
        while (zeros.back()<z_max) {
            zeros.push_back(next());
            added=true;
        }
    }

    std::pair<unsigned int, unsigned int> BesselZeros::within(double z_min, double z_max)
    {
        compute_up_to(z_max);
        auto first = std::find_if(zeros.begin(), zeros.end(), [&](double& z){return z>z_min;});
        auto last = std::find_if(first, zeros.end(), [&](double& z){return z>z_max;})--;


        return std::make_pair(
                std::distance(zeros.begin(), first),
                std::distance(zeros.begin(), last)
        );
    }

    double_t BesselZeros::operator[](unsigned int n)
    {
        while (zeros.size()<=n) {
            zeros.push_back(next());
        }
        return zeros[n];
    }

    SphericalBesselZeros::SphericalBesselZeros(unsigned int l)
            :BesselZeros(l+0.5) { }

}}