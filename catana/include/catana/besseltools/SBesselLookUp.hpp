//
// Created by Michael Bühlmann on 19/02/16.
//

#ifndef CATANA_SBESSELLOOKUP_HPP
#define CATANA_SBESSELLOOKUP_HPP

#include <memory>

namespace catana {namespace besseltools {

        class SBesselLookUp {
        public:
            SBesselLookUp(unsigned short l, unsigned short nmax, unsigned int n_interp);

            double operator()(const double& z) const;

            double get_zmax() const;

        private:
            void load_lut();

        private:
            unsigned short l;
            double z_max;
            double delta_z_inv;
            unsigned int n_interp;
            std::unique_ptr<double[]> lut;
        };

}}
#endif //CATANA_SBESSELLOOKUP_HPP
