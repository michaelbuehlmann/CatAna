//
// Created by Michael Bühlmann on 01/11/15.
//

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>

namespace catana { namespace besseltools {

    using double_t = double;


    class BesselZeros {
    public:
        BesselZeros(double_t l);
        double_t operator[](unsigned int n);
        void compute_up_to(double_t z_max);

        std::pair<unsigned int, unsigned int> within(double z_min, double z_max);

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

}}
#endif //TOOLS_HPP
