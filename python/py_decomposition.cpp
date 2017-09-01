#include <catana/catana.hpp>
#include <catana/tools/random.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/complex.h>


namespace py = pybind11;
using namespace catana;

PYBIND11_MODULE(decomposition, m) {
  m.doc() = "python binding for SFB decomposition of particle positions (part of CatAna)";

  // Initialization of random numbers. Once with given seed, once random seed
  m.def("init_random", py::overload_cast<unsigned int>(&init_random), R"pbdoc(
      Sets a specific seed for the random number generator in the io module (used for filtering)

      Parameters:
          seed (unsigned int): the seed for the RNG.
  )pbdoc", py::arg("seed"));
  m.def("init_random", py::overload_cast<>(&init_random), R"pbdoc(
      Sets a random seed for the random number generator in the io module (used for filtering)
  )pbdoc");

  py::class_<KClkk>(m, "KClkk", R"pbdoc(
          Return type of SFB decomposition methods
      )pbdoc")
      .def_readonly("k_ln", &KClkk::k_ln, R"pbdoc(
          contains the k values at which C_l(k,k) is evaluated
      )pbdoc")
      .def_readonly("c_ln", &KClkk::c_ln, R"pbdoc(
          contains the C_l(k,k) values

          index 0: 0 < l < lmax
          index 1: 0 < n < nmax
      )pbdoc")
      .def_readonly("f_lmn", &KClkk::f_lmn, R"pbdoc(
          contains the f_lm(k,k) values

          index 0: 0 < l < lmax
          index 1: 0 < m <= l
          index 2: 0 < n < nmax
      )pbdoc")
      .def("savetxt", &KClkk::savetxt, R"pbdoc(
          Save k_ln and c_ln to textfile

          This will generate the following files:
            * filename_base + ".k_ln": the k_ln values (row: l, column: n)
            * filename_base + ".c_ln": the C_ln values (row: l, column: n)

          Parameters:
              filename_base (str): base path where the files will be created
      )pbdoc", py::arg("filename_base"));

  py::class_<Analyzer>(m, "Analyzer", R"pbdoc(
          Decomposition workflow which combines I/O and SFB transformation.

          The Analyzer class combines `Source` with `Filter` instances and allows for direct computation of the SFB
          transform.
      )pbdoc")
      .def(py::init<io::Source *>(), R"pbdoc(
          Constructor

          Parameters:
              source (Source): the (file) source from which to read
      )pbdoc", py::arg("source"))
      .def("set_source", &Analyzer::set_source, R"pbdoc(
          reset the source
      )pbdoc", py::arg("source"))
      .def("set_subsample_size", &Analyzer::set_subsample_size, R"pbdoc(
          set the number of points that are randomly sampled

          If not set, all points that pass filtering will be taken into account for analysis. Note that random
          subsampling is done after filtering.
      )pbdoc", py::arg("subsample_size"))
      .def("add_filter", &Analyzer::add_filter, R"pbdoc(
          Add a Filter to the pipeline.

          Filters will be applied sequentially in the order that they were added to the Analyzer.
      )pbdoc", py::arg("Filter"))
      .def("compute_sfb", &Analyzer::compute_sfb, R"pbdoc(
          Compute the SFB transform using the brute-force method

          Parameters:
              lmax (int): largest multipole to compute
              nmax (int): largest k-index to compute
              rmax (float): maximal radius of data
              store_flmn (bool): if ``True`` the output will contain the f_lmn coefficients, otherwise only the C_ln
              verbose (bool): if ``True`` print additional logging information to stdout

          Warning:
              This will read and filter the source input and compute the Spherical Fourier Bessel decomposition from
              the individual points directly without making use of the speed-up through the pixelization scheme.

              NOT RECOMMENDED!
      )pbdoc", py::arg("lmax"), py::arg("nmax"), py::arg("rmax"), py::arg("store_flmn") = false,
          py::arg("verbose") = true)
      .def("compute_sfb_pixelized", &Analyzer::compute_sfb_pixelized, R"pbdoc(
          Compute the SFB transform using the pixelization method

          Parameters:
              lmax (int): largest multipole to compute
              nmax (int): largest k-index to compute
              rmax (float): maximal radius of data
              nside (int): HEALPix resolution, must be a power of 2
              store_flmn (bool): if ``True`` the output will contain the f_lmn coefficients, otherwise only the C_ln
              verbose (bool): if ``True`` print additional logging information to stdout
      )pbdoc",
           py::arg("lmax"), py::arg("nmax"), py::arg("rmax"), py::arg("nside"),
           py::arg("store_flmn") = false, py::arg("verbose") = true);

  m.def("sfb_decomposition", py::overload_cast<const PointContainer&, unsigned short, unsigned short, double, bool, bool>(
      &sfb_decomposition), R"pbdoc(
compute the SFB transform on the `PointContainer`

Warning:
    This method uses the slow brute-force method. Consider using a `PixelizedPointContainer` instead

Parameters:
    point_container (PointContainer): data to analyze
    lmax (int): largest multipole to compute
    nmax (int): largest k-index to compute
    rmax (float): maximal radius of data
    store_flmn (bool): if ``True`` the output will contain the f_lmn coefficients, otherwise only the C_ln
    verbose (bool): if ``True`` print additional logging information to stdout

Returns:
    SFB components including the k at which they were computed and the C_l(k). If store_flmn, will also save the
        f_lm(k) components
)pbdoc", py::arg("point_container"), py::arg("lmax"), py::arg("nmax"), py::arg("rmax"),
        py::arg("store_flmn") = false, py::arg("verbose") = true);

  m.def("sfb_decomposition", py::overload_cast<const PixelizedPointContainer&, unsigned short, unsigned short, double, bool, bool>(
      &sfb_decomposition), R"pbdoc(
compute the SFB transform on the `PixelizedPointContainer`

This method makes use of the fast pixelized decomposition scheme.

Parameters:
    pixelized_point_container (PixelizedPointContainer): data to analyze
    lmax (int): largest multipole to compute
    nmax (int): largest k-index to compute
    rmax (float): maximal radius of data
    store_flmn (bool): if ``True`` the output will contain the f_lmn coefficients, otherwise only the C_ln
    verbose (bool): if ``True`` print additional logging information to stdout

Returns:
    SFB components including the k at which they were computed and the C_l(k). If store_flmn, will also save the
        f_lm(k) components
)pbdoc", py::arg("pixelized_point_container"), py::arg("lmax"), py::arg("nmax"), py::arg("rmax"),
        py::arg("store_flmn") = false, py::arg("verbose") = true);
}