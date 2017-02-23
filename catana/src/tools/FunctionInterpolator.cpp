#include <catana/tools/FunctionInterpolator.hpp>
#include <stdexcept>

namespace catana {

  FunctionInterpolator::FunctionInterpolator(std::function<double(double)> fct,
                                             unsigned int interpolation_points, double x_min, double x_max,
                                             bool parallel)

      : interpolation_points(interpolation_points), x_min(x_min), x_max(x_max),
        y_values(new double[interpolation_points]) {
    double dx = (x_max - x_min) / (interpolation_points - 1);
    dx_inv = 1. / dx;

#pragma omp parallel for if(parallel)
    for(size_t i = 0; i < interpolation_points; ++i) {
      y_values[i] = fct(x_min + i * dx);
    }
  }

  double FunctionInterpolator::operator()(double x) const {
    if((x < x_min) || (x >= x_max)) {
      throw std::domain_error("argument outside range for which this FunctionInterpolator is defined");
    }
    x -= x_min;
    double i_approx = x * dx_inv;
    unsigned int i = static_cast<unsigned int>(i_approx);

    return y_values[i] + (y_values[i + 1] - y_values[i]) * (i_approx - i);
  }

  double FunctionInterpolator::operator[](unsigned int index) const {
    return y_values[index];
  }

  unsigned int FunctionInterpolator::get_index(double x) const {
    if((x < x_min) || (x >= x_max)) {
      throw std::domain_error("argument outside range for which this FunctionInterpolator is defined");
    }
    x -= x_min;
    return static_cast<unsigned int>(x * dx_inv);
  }

  FunctionInterpolator::FunctionInterpolator(FunctionInterpolator&& other) {
    interpolation_points = other.interpolation_points;
    x_min = other.x_min;
    x_max = other.x_max;
    dx_inv = other.dx_inv;
    std::swap(y_values, other.y_values);
  }

  FunctionInterpolator& FunctionInterpolator::operator=(FunctionInterpolator other) {
    interpolation_points = other.interpolation_points;
    x_min = other.x_min;
    x_max = other.x_max;
    dx_inv = other.dx_inv;
    std::swap(y_values, other.y_values);
    return *this;
  }


}