//
// Created by Michael Bühlmann on 14/02/16.
//

#include <gtest/gtest.h>
#include <catana/tools/gsl_function_interpolate.hpp>
#include <cmath>

TEST(Interpolation, Basic){
    auto fct = [](double x){return std::sin(x);};
    FunctionInterpolator fct_interp(fct, 10000, 0, 10);

    std::vector<double> x_array = {0, 1, 1.1, 1.2, 5, 7, 9.9, 10};
    for(auto x: x_array){
        EXPECT_FLOAT_EQ(fct(x), fct_interp(x)) << "at x=" << x;
    }

    ASSERT_DEATH(double t = fct_interp(10.5), "");
    ASSERT_DEATH(double t = fct_interp(-0.1), "");
}