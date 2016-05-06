//
// Created by Michael Bühlmann on 29/01/16.
//

#include <functional>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include <catana/besseltools.hpp>

namespace py = pybind11;
using namespace catana;


PYBIND11_PLUGIN(besseltools) {
    py::module m("besseltools", "python binding to besseltools library (part of CatAna)");

    py::class_<besseltools::SphericalBesselZeros>(m, "SphericalBesselZeros")
            .def(py::init<const double&>())
            .def("__getitem__", &besseltools::SphericalBesselZeros::operator[],
                    "Return n-th zero",
                    py::arg("n"))
            .def("compute_up_to", &besseltools::SphericalBesselZeros::compute_up_to,
                    "Precompute first n zeros",
                    py::arg("n"));

    m.def("double_sbessel_integrator",  // Vectorized in k1, k2 (can give numpy array directly)
            [](std::function<double(double)> w, unsigned int l, double r_max, py::array_t<double> k1, py::array_t<double> k2) {
                auto stateful_closure = [w, l, r_max](double k1, double k2) {
                    return besseltools::double_sbessel_integrator(w, l, r_max, k1, k2);
                };
                return py::vectorize(stateful_closure)(k1, k2);
            }
    );

    m.def("double_sbessel_integrator",  // Vectorized in k1, k2 (can give numpy array directly)
            [](FunctionInterpolator* fi_p, unsigned int l, double r_max, py::array_t<double> k1, py::array_t<double> k2) {
            besseltools::SphericalBesselZeros bz(l);
                auto stateful_closure = [fi_p, &bz, l, r_max](double k1, double k2) {
                    return besseltools::double_sbessel_integrator_bz([&](double r){return fi_p->operator()(r);}, l, bz, r_max, k1, k2);
                };
                return py::vectorize(stateful_closure)(k1, k2);
            }
    );

    return m.ptr();
}