#include <catana/iotools.hpp>
#include <catana/decomposition.hpp>

#include <Eigen/Dense>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace catana;


PYBIND11_PLUGIN(decomp_core)
{
    py::module m("decomp_core", "python binding for SFB decomposition of particle positions (part of CatAna)");

    py::class_<KClkk>(m, "KClkk")
            .def_readonly("k_ln", &KClkk::k_ln)
            .def_readonly("c_ln", &KClkk::c_ln)
            .def_readonly("f_lmn", &KClkk::f_lmn)
            .def("savetxt", &KClkk::savetxt);

    py::class_<Analyzer>(m, "Analyzer")
            .def(py::init<io::Source*, double>())
            .def("set_source", &Analyzer::set_source)
            .def("add_filter", &Analyzer::add_filter)
            .def("compute_sfb", &Analyzer::compute_sfb)
            .def("compute_sfb_pixelized", &Analyzer::compute_sfb_pixelized);

    m.def("sfb_decomposition",
            [](ObjectContainer& oc, unsigned short lmax, unsigned short nmax, double rmax, double window_volume, bool store_flmn, bool verbose)
            {
                return sfb_decomposition(oc, lmax, nmax, rmax, window_volume, store_flmn, verbose);
            },
            "compute the C_l(k,k) of the SFB decomposition.",
            py::arg("ObjectContainer"), py::arg("lmax"), py::arg("nmax"), py::arg("rmax"), py::arg("window_volume"),
            py::arg("store_flmn")=false, py::arg("verbose")=true);

    m.def("sfb_decomposition",
            [](PixelizedObjectContainer& pic_oc, unsigned short lmax, unsigned short nmax, double rmax, double window_volume, bool store_flmn, bool verbose)
            {
                return sfb_decomposition(pic_oc, lmax, nmax, rmax, window_volume, store_flmn, verbose);
            },
            "compute the C_l(k,k) of the SFB decomposition.",
            py::arg("PixelizedObjectContainer"), py::arg("lmax"), py::arg("nmax"), py::arg("rmax"), py::arg("window_volume"),
            py::arg("store_flmn")=false, py::arg("verbose")=true);

    return m.ptr();
}