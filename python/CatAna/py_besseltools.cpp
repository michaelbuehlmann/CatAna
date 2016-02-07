//
// Created by Michael Bühlmann on 29/01/16.
//

#include <functional>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include <catana/besseltools.hpp>

namespace py = pybind11;

PYBIND11_PLUGIN(pybesseltools) {
    py::module m("pybesseltools", "python binding to besseltools library (part of CatAna)");

    py::class_<BesselZeros>(m, "BesselZeros")
            .def(py::init<const double&>())
            .def("__getitem__", &BesselZeros::operator[],
                    "Return n-th zero",
                    py::arg("n"))
            .def("compute_up_to", &BesselZeros::compute_up_to,
                    "Precompute first n zeros",
                    py::arg("n"));

    py::class_<SphericalBesselZeros>(m, "SphericalBesselZeros", py::base<BesselZeros>())
            .def(py::init<const double&>());

    py::class_<SBesselTransformedFunction>(m, "SBesselTransformedFunction")
            .def(py::init<std::function<double(double)>, const unsigned int&, const double&>())
            .def("__call__", &SBesselTransformedFunction::operator());

    m.def("double_sbessel_integrator", &double_sbessel_integrator,
            "compute integral [0,Rmax] over w(r)*j_l(k1*r)*j_l(k2*r)",
            py::arg("w"), py::arg("l"), py::arg("Rmax"), py::arg("k1"), py::arg("k2"));

    return m.ptr();
}