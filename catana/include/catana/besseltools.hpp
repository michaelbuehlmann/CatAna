//
// Created by Michael Bühlmann on 01/11/15.
//

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>
#include <cmath>
#include <functional>
using double_t = double;


class BesselZeros{
public:
    BesselZeros(double_t l);
    double_t operator[](unsigned int n);
    void compute_up_to(double_t z_max);
private:
    double_t l;
    std::vector<double_t> zeros;

    double_t first_zero();
    double_t second_zero();
    double_t newton_iterate(double_t z);
    double_t next();
    unsigned int pos;
};


class SphericalBesselZeros : public BesselZeros {
public:
    SphericalBesselZeros(unsigned int l);
};


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

#endif //TOOLS_HPP
