//
// Created by Michael Bühlmann on 01/11/15.
//

#include "catana/Catalog.hpp"
#include "catana/decomposition/sfb_decomposition.hpp"

unsigned long Catalog::size() const
{
    return cat_rtp.size();
}

KClkk Catalog::compute_SFB_raw(unsigned short lmax, unsigned short nmax) const
{
    return decomp_SFB_raw(cat_rtp, lmax, nmax, window.get_Rmax(), window.volume());
}

KClkk Catalog::compute_SFB_reverse(unsigned short lmax, unsigned short nmax, unsigned int NSide) const
{
    return decomp_SFB_reverse(cat_rtp, lmax, nmax, window.get_Rmax(), window.volume(), NSide);
}

void Catalog::add_object(const Object& object)
{
    cat_rtp.push_back(object);
}
