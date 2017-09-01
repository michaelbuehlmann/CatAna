#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>


namespace catana { namespace besseltools {

  class SphericalBesselZeros {
  public:
    SphericalBesselZeros(unsigned int l);

    double operator[](unsigned int n);

    void compute_up_to(double z_max);

    unsigned int next_zero(double z);

  private:
    unsigned int l;
    std::vector<double> zeros;

    double first_zero();

    double second_zero();

    double newton_iterate(double z);

    double next();
  };

}}
#endif //TOOLS_HPP
