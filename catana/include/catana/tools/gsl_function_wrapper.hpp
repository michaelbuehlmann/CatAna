#ifndef CATANA_APP_GSL_FUNCTION_WRAPPER_HPP
#define CATANA_APP_GSL_FUNCTION_WRAPPER_HPP

#include <gsl/gsl_integration.h>


namespace catana {

// Some wrapper for function -> gsl_function pointers
  template<typename F>
  class gsl_function_pp : public gsl_function {
  public:
    gsl_function_pp(const F& func)
        : _func(func) {
      function = &gsl_function_pp::invoke;
      params = this;
    }

  private:
    const F& _func;

    static double invoke(double x, void *params) {
      return static_cast<gsl_function_pp *>(params)->_func(x);
    }
  };

}
#endif //CATANA_APP_GSL_FUNCTION_WRAPPER_HPP
