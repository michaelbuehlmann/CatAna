//
// Created by Michael Bühlmann on 18/11/15.
//

#ifndef CATANA_APP_FIELD_HPP
#define CATANA_APP_FIELD_HPP

#include <functional>
#include <complex>
#include "tools/fftw_field.hpp"
#include "decomposition/return_types.hpp"
#include "Window.hpp"

class Catalog;

//! Class representing a overdensity field which can be Fourier-Transformed.
class Field {
public:
    using double_t = double;
    using complex_t = std::complex<double_t>;

    Field(size_t const M, double_t const L, bool inline_fftw=false);
    Field(size_t const M, double_t const L, double_t * const realspace_field, bool inline_fftw=false);

    void initialize_with_powerspectrum(std::function<double_t(double_t)>);
    KPkk compute_powerspectrum(unsigned short M, unsigned short bin_number);

    field_wrapper<double_t> real_field;
    field_wrapper<complex_t> complex_field;

    void forward_transform();
    void backward_transform();

    Catalog poisson_sample(size_t N, const Window& w);

private:
    fftw_field<double_t> rdata;
    fftw_field<complex_t> cdata;

    fftw_plan forward_plan;
    fftw_plan backward_plan;

    bool realstate;
    size_t M;
    double_t L;

    bool inline_fftw;
};

#endif //CATANA_APP_FIELD_HPP
