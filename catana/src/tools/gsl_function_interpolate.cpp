//
// Created by Michael Bühlmann on 14/02/16.
//

#include <catana/tools/gsl_function_interpolate.hpp>
#include <cassert>

namespace catana {
    FunctionInterpolator::FunctionInterpolator(std::function<double(double)> fct, size_t interpolation_points,
            double min,
            double max)
            :interpolation_points(interpolation_points)
    {
        assert(min<max);
        assert(interpolation_points>=2);

        x_values = new double[interpolation_points];
        y_values = new double[interpolation_points];

        double delta_x = (max-min)/(interpolation_points-1);  // Such that max is last point
        double x;

        for (size_t i = 0; i<=interpolation_points; ++i) {
            x = min+i*delta_x;
            x_values[i] = x;
            y_values[i] = fct(x);
        }

        // GSL stuff
        interp_p = gsl_interp_alloc(gsl_interp_linear, interpolation_points);
        gsl_interp_init(interp_p, x_values, y_values, interpolation_points);
        accel_p = gsl_interp_accel_alloc();
    }

    FunctionInterpolator::~FunctionInterpolator()
    {
        gsl_interp_accel_free(accel_p);
        gsl_interp_free(interp_p);
        delete[] x_values;
        delete[] y_values;
    }

    double FunctionInterpolator::operator()(double r)
    {
        return gsl_interp_eval(interp_p, x_values, y_values, r, accel_p);
    }

}