#include <catana/iotools.hpp>
#include <catana/decomposition.hpp>

#include <Eigen/Dense>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace catana;

// Provide a random number generator
std::random_device rand_dev;
std::mt19937 rng;


PYBIND11_PLUGIN(decomp_core)
{
    py::module m("decomp_core", "python binding for SFB decomposition of particle positions (part of CatAna)");

    // Initialization of random numbers. Once with given seed, once random seed
    m.def("init_random", [&](unsigned int seed){
                rng.seed(seed); },
            py::arg("seed"));
    m.def("init_random", [&](){
        std::array<int, 624> seed_data;
        std::generate(seed_data.begin(), seed_data.end(), std::ref(rand_dev));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        rng.seed(seq); });

    py::class_<KClkk>(m, "KClkk")
            .def_readonly("k_ln", &KClkk::k_ln)
            .def_readonly("c_ln", &KClkk::c_ln)
            .def_readonly("f_lmn", &KClkk::f_lmn)
            .def("savetxt", &KClkk::savetxt,
                    "Save k_ln and c_ln to textfile",
                    py::arg("filename_base"));

    py::class_<Analyzer>(m, "Analyzer")
            .def(py::init<io::Source*, double>(),
                    py::arg("Source"), py::arg("window_volume"))
            .def("set_source", &Analyzer::set_source,
                    py::arg("Source"))
            .def("set_subsample_size", &Analyzer::set_subsample_size,
                    py::arg("subsample_size"))
            .def("add_filter", &Analyzer::add_filter,
                    py::arg("Filter"))
            .def("compute_sfb", &Analyzer::compute_sfb,
                    "Compute the C_l(k,k) for the filtered source using 'RAW' method",
                    py::arg("lmax"), py::arg("nmax"), py::arg("rmax"),
                    py::arg("store_flmn")=false, py::arg("verbose")=true)
            .def("compute_sfb_pixelized", &Analyzer::compute_sfb_pixelized,
                    "Compute the C_l(k,k) for the filtered source using 'REVERSE_FFT' method",
                    py::arg("lmax"), py::arg("nmax"), py::arg("rmax"), py::arg("nside"),
                    py::arg("store_flmn")=false, py::arg("verbose")=true);

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