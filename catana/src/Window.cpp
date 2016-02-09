//
// Created by Michael Bühlmann on 29/01/16.
//

#include <catana/Window.hpp>
#include <catana/tools/gsl_function_wrapper.hpp>

#include <cmath>
#include <limits>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>



typedef std::numeric_limits<double_t> double_t_lim;

Window::Window()
        : windowtype(WindowType::numeric), scale_factor(0), window_function([](double_t r){return 1.;}), r_max(0)
{}

Window::Window(double_t r_max)
        : Window(WindowType::tophat, r_max, r_max)
{}

Window::Window(WindowType windowtype, double_t scale_factor, double_t r_max)
        : windowtype(windowtype), scale_factor(scale_factor), r_max(r_max)
{
    switch(windowtype){
    case WindowType::gauss:
        window_function = [&](double_t r){return std::exp(-std::pow(r/scale_factor, 2));};
        break;
    case WindowType::tophat:
        window_function = [&](double_t r){return r<scale_factor ? 1. : 0.;};
        break;
    default:
        throw WindowTypeNotSupportedException();
    }
}

Window::Window(std::function<double_t(double_t)> window_function, double_t r_max)
        : windowtype(WindowType::numeric), scale_factor(0), window_function(window_function), r_max(r_max)
{}

double_t Window::operator()(double_t t) const
{
    return window_function(t);
}

double_t Window::pk_pkk_coefficient() const
{
    // TODO: implement pk_pkk coefficient
    return 0;
}

double_t Window::pk_clkk_coefficient() const
{
    double_t coeff;
    switch(windowtype) {
    case WindowType::gauss:
        coeff = scale_factor/(std::sqrt(8*M_PI));
        break;
    case WindowType::tophat:
        coeff = scale_factor/M_PI * std::sqrt(3./M_PI);
        break;
    default:
        throw WindowTypeNotSupportedException();
    }

    return coeff;
}

double_t Window::volume() const
{
    auto integrand = [&](double_t r){
        return 4*M_PI*r*r * window_function(r);
    };
    gsl_function_pp<decltype(integrand)> Fp(integrand);
    gsl_function *F = static_cast<gsl_function*>(&Fp);

    double result, error;
    int exit_code;
    auto workspace = gsl_integration_workspace_alloc(2000);

    exit_code = gsl_integration_qag(F, 0, r_max, 0, 100 * double_t_lim::epsilon(), 2000, 2, workspace, &result,
            &error);
    gsl_integration_workspace_free(workspace);

    return result;
}

