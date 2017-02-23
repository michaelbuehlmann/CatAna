#include <catana/io.hpp>
#include <catana/decomposition.hpp>
#include <catana/tools/random.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;
using namespace catana;


PYBIND11_PLUGIN(decomposition) {
  py::module m("decomposition", "python binding for SFB decomposition of particle positions (part of CatAna)");

  // Initialization of random numbers. Once with given seed, once random seed
  m.def("init_random", py::overload_cast < unsigned
  int > (&init_random),
      py::arg("seed"));
  m.def("init_random", py::overload_cast<>(&init_random));

  py::class_<KClkk>(m, "KClkk")
      .def_readonly("k_ln", &KClkk::k_ln)
      .def_readonly("c_ln", &KClkk::c_ln)
      .def_readonly("f_lmn", &KClkk::f_lmn)
      .def("savetxt", &KClkk::savetxt,
           "Save k_ln and c_ln to textfile",
           py::arg("filename_base"));

  py::class_<Analyzer>(m, "Analyzer")
      .def(py::init<io::Source *, double>(),
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
           py::arg("store_flmn") = false, py::arg("verbose") = true)
      .def("compute_sfb_pixelized", &Analyzer::compute_sfb_pixelized,
           "Compute the C_l(k,k) for the filtered source using 'REVERSE_FFT' method",
           py::arg("lmax"), py::arg("nmax"), py::arg("rmax"), py::arg("nside"),
           py::arg("store_flmn") = false, py::arg("verbose") = true);

  m.def("sfb_decomposition",
        [](ObjectContainer& oc, unsigned short lmax, unsigned short nmax, double rmax, double window_volume,
           bool store_flmn, bool verbose) {
          return sfb_decomposition(oc, lmax, nmax, rmax, window_volume, store_flmn, verbose);
        },
        "compute the C_l(k,k) of the SFB decomposition.",
        py::arg("ObjectContainer"), py::arg("lmax"), py::arg("nmax"), py::arg("rmax"), py::arg("window_volume"),
        py::arg("store_flmn") = false, py::arg("verbose") = true);

  m.def("sfb_decomposition",
        [](PixelizedObjectContainer& pic_oc, unsigned short lmax, unsigned short nmax, double rmax,
           double window_volume, bool store_flmn, bool verbose) {
          return sfb_decomposition(pic_oc, lmax, nmax, rmax, window_volume, store_flmn, verbose);
        },
        "compute the C_l(k,k) of the SFB decomposition.",
        py::arg("PixelizedObjectContainer"), py::arg("lmax"), py::arg("nmax"), py::arg("rmax"),
        py::arg("window_volume"),
        py::arg("store_flmn") = false, py::arg("verbose") = true);

  return m.ptr();
}