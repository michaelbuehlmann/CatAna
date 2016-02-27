//
// Created by Michael Bühlmann on 19/02/16.
//

#ifndef CATANA_SBESSELLOOKUP_HPP
#define CATANA_SBESSELLOOKUP_HPP

#include <memory>
#include "../tools/FunctionInterpolator.hpp"

namespace catana {namespace besseltools {

        class SBesselInterpolator : public FunctionInterpolator{
        public:
            SBesselInterpolator(unsigned short l, unsigned short nmax, unsigned int n_interp);
            double operator()(const double& z) const;
            double get_zmax() const;

        private:
            unsigned short l;
        };

}}
#endif //CATANA_SBESSELLOOKUP_HPP
