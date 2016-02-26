//
// Created by Michael Bühlmann on 14/02/16.
//

#ifndef CATANA_GSL_FUNCTION_INTERPOLATE_HPP
#define CATANA_GSL_FUNCTION_INTERPOLATE_HPP

#include <functional>
#include <gsl/gsl_interp.h>

// TODO: fix default GSL errorhandler (i.e. force interpolation to die outside of valid range)

namespace catana {
    class FunctionInterpolator {
    public:
        FunctionInterpolator(std::function<double(double)> fct, size_t interpolation_points, double min, double max);

        ~FunctionInterpolator();

        // Non copyable, non assignable;
        FunctionInterpolator(FunctionInterpolator const&) = delete;

        FunctionInterpolator& operator=(FunctionInterpolator const&) = delete;

        // Call operator
        double operator()(double r);

    private:
        size_t interpolation_points;
        gsl_interp* interp_p;
        gsl_interp_accel* accel_p;
        double* x_values;
        double* y_values;
    };
}

#endif //CATANA_GSL_FUNCTION_INTERPOLATE_HPP
