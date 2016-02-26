////
//// Created by Michael Bühlmann on 09/12/15.
////
//
//#ifndef CATANA_APP_WINDOW_HPP
//#define CATANA_APP_WINDOW_HPP
//
//#include <functional>
//#include <string>
//#include <stdexcept>
//
//using double_t = double;
//
//enum class WindowType {
//    numeric,
//    gauss,
//    tophat
//};
//
//class Window {
//public:
//    Window();
//    Window(double_t r_max);
//    Window(WindowType windowtype, double_t scale_factor, double_t r_max);
//    Window(std::function<double_t(double_t)> window_function, double_t r_max);
//
//    double_t operator()(double_t) const;
//    double_t pk_pkk_coefficient() const;
//    double_t pk_clkk_coefficient() const;
//
//    double_t volume() const;
//
//    double_t get_Rmax() const
//    {
//        return r_max;
//    }
//
//private:
//    std::function<double_t(double_t)> window_function;
//    WindowType windowtype;
//    double_t scale_factor;
//    double_t r_max;
//};
//
//class WindowTypeNotSupportedException : public std::exception {
//public:
//    WindowTypeNotSupportedException(const char * error = "WindowType not supported (not gaussian or tophat?)") {
//        errorMessage = error;
//    }
//    const char * what() const noexcept {
//        return errorMessage.c_str();
//    }
//private:
//    std::string errorMessage;
//};
//
//#endif //CATANA_APP_WINDOW_HPP
