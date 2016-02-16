//
// Created by Michael Bühlmann on 15/02/16.
//

#include <catana/objects/PixelizedObjectContainer.hpp>
#include <cassert>
#include <algorithm>
#include <numeric>

bool is_power_of_2(const int& n) {
    return (((n & (n-1))==0));
}

PixelizedObjectContainer::PixelizedObjectContainer(unsigned int nside)
        :hp_base(nside, RING, SET_NSIDE), nside(nside)
{
    assert(is_power_of_2(nside));
    resize(hp_base.Npix());
    for(int i=0; i<hp_base.Npix(); ++i){
        operator[](i).p = hp_base.pix2ang(i);
    }
}

PixelizedObjectContainer::PixelizedObjectContainer(unsigned int nside, const ObjectContainer& oc)
        :PixelizedObjectContainer(nside)
{
    std::for_each(oc.begin(), oc.end(), [this](const Object& object){this->add_object(object);});
}

void PixelizedObjectContainer::add_object(const Object& object) {int i = hp_base.ang2pix(object.p);
    operator[](i).push_back(object.r);
}

unsigned int PixelizedObjectContainer::get_nside() const
{
    return nside;
}

size_t PixelizedObjectContainer::get_nobjects() const
{
    return std::accumulate(begin(), end(), 0ULL, [&](const long sum, const PixelObjects& pix_objs){return sum+pix_objs.size();});
}


