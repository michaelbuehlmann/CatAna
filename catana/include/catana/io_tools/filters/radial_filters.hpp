//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_RADIAL_FILTERS_HPP
#define CATANA_RADIAL_FILTERS_HPP

#include "../Filter.hpp"
#include <functional>
#include <catana/tools/FunctionInterpolator.hpp>
#include <memory>

namespace catana{ namespace io {

        //! A generic radial filter. Window functionreturns the probability of object beeing accepted.
        class GenericRadialWindowFunctionFilter : public Filter {
        public:
            GenericRadialWindowFunctionFilter(std::function<double(double)> window_function);

            GenericRadialWindowFunctionFilter(
                    std::function<double(double)> window_function,
                    size_t interpolation_points, double min, double max
            );

            bool filter(Object& object) override;

        private:
            std::function<double(double)> window_function;
            std::uniform_real_distribution<double> random_dist;
            std::unique_ptr<FunctionInterpolator> wfct_interp;
        };

        //! A Gaussian window function with scale R0.
        class GaussianRadialWindowFunctionFilter : public GenericRadialWindowFunctionFilter {
        public:
            GaussianRadialWindowFunctionFilter(double R0);
        };

        //! A radial tophat window function, only keeps objects within R0
        class TophatRadialWindowFunctionFilter : public Filter {
        public:
            TophatRadialWindowFunctionFilter(double R0);

            virtual bool filter(Object& object);

        private:
            double R0;

        };


}}
#endif //CATANA_RADIAL_FILTERS_HPP
