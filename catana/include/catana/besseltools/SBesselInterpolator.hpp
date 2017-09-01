#ifndef CATANA_SBESSELLOOKUP_HPP
#define CATANA_SBESSELLOOKUP_HPP

#include <vector>
#include <memory>
#include "../tools/FunctionInterpolator.hpp"


namespace catana { namespace besseltools {

  class SBesselInterpolator : public FunctionInterpolator {
  public:
    SBesselInterpolator(unsigned short l, double z_max, unsigned int interpolation_points);

    double operator()(const double& z) const;

    double get_zmax() const;

  private:
    unsigned short l;
  };


  SBesselInterpolator
  SBesselInterpolator_uptozero(unsigned short l, unsigned short nmax, unsigned int interpolation_points);


  class SBesselIFunction {
  public:
    SBesselIFunction(double z_max, unsigned int interpolation_points);

    void initialize(unsigned int l);

    void deallocate(unsigned int l);

    double operator()(unsigned int l, double z);

  private:
    double z_max;
    unsigned int interpolation_points;
    std::vector<std::pair<std::unique_ptr<SBesselInterpolator>, bool>> interpolators;
  };
}}
#endif //CATANA_SBESSELLOOKUP_HPP
