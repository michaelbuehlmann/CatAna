//
// Created by Michael Bühlmann on 14/02/16.
//

#ifndef CATANA_GSL_FUNCTION_INTERPOLATE_HPP
#define CATANA_GSL_FUNCTION_INTERPOLATE_HPP

#include <functional>
#include <memory>

namespace catana {

    class FunctionInterpolator {
    public:
        FunctionInterpolator(std::function<double(double)> fct, unsigned int interpolation_points, double x_min,
                double x_max);

        // Non copyable, non assignable;
        FunctionInterpolator(FunctionInterpolator const&) = delete;
        FunctionInterpolator& operator=(FunctionInterpolator const&) = delete;

        // Call operator
        double operator()(double x) const;
        double operator[](unsigned int index) const;
        unsigned int get_index(double x) const;

    protected:
        unsigned int interpolation_points;
        double x_min, x_max;
        double dx_inv;
        std::unique_ptr<double[]> y_values;
    };

}
#endif //CATANA_GSL_FUNCTION_INTERPOLATE_HPP
