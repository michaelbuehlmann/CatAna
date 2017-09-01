#include <functional>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include <catana/catana.hpp>

namespace py = pybind11;
using namespace catana;


PYBIND11_MODULE(besseltools, m) {
    m.doc() = "python binding to besseltools library (part of CatAna)";

    py::class_<besseltools::SphericalBesselZeros>(m, "SphericalBesselZeros", R"pbdoc(
            A root generator for spherical Bessel functions.

            The class is initialized for a specific multipole l. The zeros are computed iteratively and the results
            are stored so that they only have to be computed once. I.e if the n-th zero is requested and it has not
            been computed before, start at the highest computed zero and compute all zeros up to the n-th.
        )pbdoc")
        .def(py::init<unsigned int>(), R"pbdoc(
            Constructor

            Parameters:
                l (int): multipole of the spherical Bessel function
        )pbdoc", py::arg("l"))
        .def("__getitem__", &besseltools::SphericalBesselZeros::operator[], R"pbdoc(
            return n-th zero

            Parameters:
                n (int): index of zero crossing

            Returns:
                float: n-th root of the spherical Bessel function with multipole l
        )pbdoc", py::arg("n"))
        .def("compute_up_to", &besseltools::SphericalBesselZeros::compute_up_to, R"pbdoc(
            precompute first n zeros

            Parameters:
                n (int): number of zeros to compute
        )pbdoc", py::arg("n"));


    m.def("double_sbessel_integrator",  // Vectorized in k1, k2 (can give numpy array directly)
        [](std::function<double(double)> w, unsigned int l, double r_max, py::array_t<double> k1, py::array_t<double> k2) {
            auto stateful_closure = [w, l, r_max](double k1, double k2) {
                return besseltools::double_sbessel_integrator(w, l, r_max, k1, k2);
            };
            return py::vectorize(stateful_closure)(k1, k2);
        }, R"pbdoc(
Compute an integral containing 2 spherical Bessel functions with the same multipole.

Computes :math:`\int_0^{r_{max}} \; w(r) \; j_l(k_1 r) \; j_l(k_2 r) \;\; dr`

Parameters:
    w (callable): function to integrate (see equation)
    l (int): multipole of the spherical Bessel functions
    r_max (float): upper boundary of integration
    k1 (float): frequency of one spherical Bessel function
    k2 (float): frequency of other spherical Bessel function
        )pbdoc", py::arg("w"), py::arg("l"), py::arg("r_max"), py::arg("k1"), py::arg("k2")
    );

    m.def("double_sbessel_integrator",  // Vectorized in k1, k2 (can give numpy array directly)
        [](FunctionInterpolator* fi_p, unsigned int l, double r_max, py::array_t<double> k1, py::array_t<double> k2) {
        besseltools::SphericalBesselZeros bz(l);
            auto stateful_closure = [fi_p, &bz, l, r_max](double k1, double k2) {
                return besseltools::double_sbessel_integrator_bz([&](double r){return fi_p->operator()(r);}, l, bz, r_max, k1, k2);
            };
            return py::vectorize(stateful_closure)(k1, k2);
        }, R"pbdoc(
Compute an integral containing 2 spherical Bessel functions with the same multipole, using a `FunctionInterpolator`

Computes :math:`\int_0^{r_{max}} \; w(r) \; j_l(k_1 r) \; j_l(k_2 r) \;\; dr`

Parameters:
    fi (FunctionInterpolator): function to integrate (see equation, fi(r) is an interpolator of w(r))
    l (int): multipole of the spherical Bessel functions
    r_max (float): upper boundary of integration
    k1 (float): frequency of one spherical Bessel function
    k2 (float): frequency of other spherical Bessel function
        )pbdoc", py::arg("w"), py::arg("l"), py::arg("r_max"), py::arg("k1"), py::arg("k2")
    );
}