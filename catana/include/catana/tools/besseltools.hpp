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


double_t double_sbessel_integrator(std::function<double_t(double_t)> f, unsigned int l, double_t Rmax, double_t k1, double_t k2);

#endif //TOOLS_HPP
