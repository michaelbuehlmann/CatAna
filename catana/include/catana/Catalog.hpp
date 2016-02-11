//
// Created by Michael Bühlmann on 01/11/15.
//

#ifndef CATALOG_HPP
#define CATALOG_HPP

#include "Field.hpp"
#include "Window.hpp"
#include <catana/types.hpp>

class Catalog {
public:
    KPkk compute_Pkk_raw() const;
    KPkk compute_Pkk() const;

    KClkk compute_SFB_raw(
            unsigned short lmax, unsigned short nmax
    ) const;

    KClkk compute_SFB_reverse(
            unsigned short lmax, unsigned short nmax,
            unsigned int NSide
    ) const;

    unsigned long size() const;
    void add_object(const Object&);

    Window window;
private:
    ObjectContainer cat_rtp;
};




#endif //CATALOG_HPP
