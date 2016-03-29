//
// Created by Michael Bühlmann on 15/02/16.
//

#include <catana/io/filters/radial_filters.hpp>

extern std::mt19937 rng;

namespace catana{ namespace io {

        GenericRadialWindowFunctionFilter::GenericRadialWindowFunctionFilter(
                std::function<double(double)> window_function)
                :window_function(window_function), random_dist(0, 1) { }

        GenericRadialWindowFunctionFilter::GenericRadialWindowFunctionFilter(
                std::function<double(double)> window_function,
                size_t interpolation_points, double min, double max, bool parallel_init)
                :random_dist(0, 1)
        {
            auto interp_p = new FunctionInterpolator(window_function, interpolation_points, min, max, parallel_init);
            wfct_interp.reset(interp_p);
            this->window_function = [=](double r) { return wfct_interp->operator()(r); };
        }

        bool GenericRadialWindowFunctionFilter::filter(Object& object)
        {
            return (window_function(object.r)>random_dist(rng));
        }


        GaussianRadialWindowFunctionFilter::GaussianRadialWindowFunctionFilter(double R0)
                :GenericRadialWindowFunctionFilter([R0](double r) { return std::exp(-std::pow(r/R0, 2)); }) { }

        TophatRadialWindowFunctionFilter::TophatRadialWindowFunctionFilter(double R0)
                :R0(R0) { }

        bool TophatRadialWindowFunctionFilter::filter(Object& object)
        {
            return object.r<R0;
        }

}}