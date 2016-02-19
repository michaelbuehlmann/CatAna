//
// Created by Michael Bühlmann on 19/02/16.
//

#ifndef CATANA_SBESSEL_INTEGRATOR_HPP
#define CATANA_SBESSEL_INTEGRATOR_HPP

#include <functional>
#include "BesselZeros.hpp"

class SBesselTransformedFunction {
public:
    SBesselTransformedFunction(
            std::function<double_t(double_t)> f,
            unsigned int l, double_t Rmax);

    double_t operator()(const double_t& k1, const double_t& k2);
private:
    std::function<double_t(double_t)> function;
    unsigned int l;
    SphericalBesselZeros bz;
    double_t Rmax;
};


//! Computes the integral over f(r)*j_l(k1*r)*j_l(k2*r) between [0,Rmax]
double_t double_sbessel_integrator(
        std::function<double_t(double_t)> f,
        const unsigned int& l,
const double_t& Rmax,
        double_t k1, double_t k2);

//! Computes the integral over f(r)*j_l(k1*r)*j_l(k2*r) between [0,Rmax]
/*
 * The order of bz must be of the same l!
 */
double_t double_sbessel_integrator_bz(
        std::function<double_t(double_t)> f,
        const unsigned int& l,
        SphericalBesselZeros& bz,
const double_t& Rmax,
        double_t k1, double_t k2);


#endif //CATANA_SBESSEL_INTEGRATOR_HPP
