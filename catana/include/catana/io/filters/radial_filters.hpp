#ifndef CATANA_RADIAL_FILTERS_HPP
#define CATANA_RADIAL_FILTERS_HPP

#include "../Filter.hpp"
#include <catana/tools/FunctionInterpolator.hpp>
#include <functional>
#include <memory>
#include <random>


namespace catana { namespace io {

  //! A generic radial window function filter, bases on a function returning the acceptance probability at a given radius.
  class GenericRadialWindowFunctionFilter : public Filter {
  public:

    //! Construct from window_function
    /*!
     * @param window_function a function with signature double(double r), where the return value is in [0,1] and
     * gives the probability of an point at distance r passing the filter.
     */
    GenericRadialWindowFunctionFilter(std::function<double(double)> window_function);

    //! Construct from window_function, with interpolation (good if computation is intensive)
    /*!
     * @param window_function a function with signature double(double r), where the return value is in [0,1] and
     * gives the probability of an point at distance r passing the filter.
     * @param interpolation_points number of points which are spaced equidistantly between [min, max] and between which
     * the function will be linearly interpolated
     * @param min lower bound of interpolation (runtime error if called with radius outside boundaries)
     * @param max upper bound of interpolation (runtime error if called with radius outside boundaries)
     * @param parallel_init if true, initialize interpolation points in parallel. Set to false if window_function is
     * a Python point.
     */
    GenericRadialWindowFunctionFilter(
        std::function<double(double)> window_function,
        size_t interpolation_points, double min, double max, bool parallel_init = false
    );

    //! Filtering function on Point. Returns true if point passes filter, false otherwise.
    bool filter(Point& point) override;

  private:
    std::function<double(double)> window_function;
    std::uniform_real_distribution<double> random_dist;
    std::unique_ptr<FunctionInterpolator> wfct_interp;
  };


  //! A Gaussian window function filter: acceptance probability = exp(-(r/R0)^2)
  class GaussianRadialWindowFunctionFilter : public GenericRadialWindowFunctionFilter {
  public:
    //! Construct from scale parameter R0
    GaussianRadialWindowFunctionFilter(double R0);
  };


  //! A radial tophat window function filter, acceptance probability = 1 if r<R0, else 0
  class TophatRadialWindowFunctionFilter : public Filter {
  public:
    //! Construct from scale parameter R0
    TophatRadialWindowFunctionFilter(double R0);

    //! Filtering function on Point. Returns true if point passes filter, false otherwise.
    virtual bool filter(Point& point);

  private:
    double R0;

  };


}}
#endif //CATANA_RADIAL_FILTERS_HPP
