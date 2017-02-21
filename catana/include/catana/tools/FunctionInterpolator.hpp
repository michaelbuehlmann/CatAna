#ifndef CATANA_GSL_FUNCTION_INTERPOLATE_HPP
#define CATANA_GSL_FUNCTION_INTERPOLATE_HPP

#include <functional>
#include <memory>


namespace catana {

  //! A function wrapper which does interpolation (for expensive functions) for signature double(double)
  class FunctionInterpolator {
  public:
    //! Construct from function
    /*!
     * @param fct A function with signature double(double) which is to be interpolated
     * @param interpolation_points Number of interpolation points spread equidistantly between x_min and x_max
     * @param x_min lower bound for interpolation (fails if called below)
     * @param x_max upper bound for interpolation (fails if called above)
     * @param parallel If true, class is initialized in parallel (set to false for a Python Function)
     */
    FunctionInterpolator(std::function<double(double)> fct, unsigned int interpolation_points, double x_min,
                         double x_max, bool parallel = true);

    // Movable
    FunctionInterpolator(FunctionInterpolator&& other);

    FunctionInterpolator& operator=(FunctionInterpolator other);

    //! Call operator
    double operator()(double x) const;

    //! Get function value at x=x_min + (x_max-x_min)/(interpolation_points+1)*index
    double operator[](unsigned int index) const;

    //! Get closest lower index value for x
    unsigned int get_index(double x) const;

  protected:
    unsigned int interpolation_points;
    double x_min, x_max;
    double dx_inv;
    std::unique_ptr<double[]> y_values;
  };

}
#endif //CATANA_GSL_FUNCTION_INTERPOLATE_HPP
