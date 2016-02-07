//
// Created by Michael Bühlmann on 18/11/15.
//

#include "catana/Field.hpp"
#include "catana/Catalog.hpp"
#include <cassert>
#include <cmath>
#include <random>
#include <healpix_cxx/pointing.h>
#include <healpix_cxx/vec3.h>

std::mt19937 rng;

Field::Field(size_t const M, double_t const L, bool inline_fftw)
        :M(M), L(L), inline_fftw(inline_fftw), realstate(true)
{
    cdata = fftw_field<complex_t>(M*M*(M/2+1));
    complex_field=field_wrapper<complex_t>(cdata.data, M, M/2+1);

    if(inline_fftw){
        real_field = field_wrapper<double_t>((double_t *) cdata.data, M, M, true);
    } else {
        rdata = fftw_field<double_t>(std::pow(M,3));
        real_field = field_wrapper<double_t>(rdata.data, M, M, false);
    }

    forward_plan = fftw_plan_dft_r2c_3d(M,M,M,real_field.data,(fftw_complex *)complex_field.data,FFTW_MEASURE);
    backward_plan = fftw_plan_dft_c2r_3d(M,M,M,(fftw_complex *)complex_field.data,real_field.data,FFTW_MEASURE);

    assert(forward_plan != nullptr);
    assert(backward_plan != nullptr);
}


Field::Field(size_t const M, double_t const L, double_t *const realspace_field, bool inline_fftw)
        :Field(M, L, inline_fftw)
{
    std::copy(realspace_field, realspace_field + M * M * M, real_field.begin());
}


void Field::forward_transform() {
    // TODO: normalize forward FT
    assert(realstate);
    fftw_execute(forward_plan);
    realstate=false;
}

void Field::backward_transform() {
    // TODO: normalize backwards FT
    assert(!realstate);
    fftw_execute(backward_plan);
    realstate=true;
}

void Field::initialize_with_powerspectrum(std::function<double_t(double_t)> function)
{
    // TODO: power specturm initialization
    throw "Not yet implemented.";
}

Catalog Field::poisson_sample(size_t N, const Window& w)
{

    if(!realstate){
        backward_transform();
    }
    assert(realstate);

    Catalog catalog;
    for(auto r_ptr=real_field.begin(); r_ptr != real_field.end(); ++ r_ptr) {
        // TODO: apply window function
        // TODO: check template parameter of poisson dist.
        std::poisson_distribution<unsigned int> dist(N/double(std::pow(M,3)) * (*r_ptr + 1));
        unsigned int n = dist(rng);

        if(n>0){
            auto freq = r_ptr.get_frequency();
            vec3_t<double> pos(freq.f1*L/M, freq.f2*L/M, freq.f3*L/M);
            Object obj;
            obj.p = pointing(pos);
            obj.r = pos.Length();
            for (int i = 0; i<n; ++i) {
                catalog.add_object(obj);
            }
        }
    }
    return Catalog();
}

KPkk Field::compute_powerspectrum(unsigned short M, unsigned short bin_number)
{
    if(realstate){
        forward_transform();
    }
    assert(!realstate);

    KPkk kPkk(M, L, bin_number, true);

    for(auto k_ptr = complex_field.begin(); k_ptr != complex_field.end(); ++k_ptr) {
        auto freq = k_ptr.get_frequency();
        double k = std::sqrt(std::pow(freq.f1,2)+std::pow(freq.f2,2)+std::pow(freq.f3,2)) * 2*M_PI/L;
        // TODO: power spectrum computation
    }
    return kPkk;
}
