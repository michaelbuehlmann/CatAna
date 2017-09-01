#include <catana/catana.hpp>
#include <catana/tools/random.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>


namespace py = pybind11;
using namespace catana;

// Which records will we use for HDF5 files?
using record_cf = io::CartesianRecord<float>;
using record_cd = io::CartesianRecord<double>;
using record_sf = io::SphericalRecord<float>;
using record_sd = io::SphericalRecord<double>;
using record_sd_3dex = io::SphericalRecord<double, io::SphericalTextFormat::THREEDEX>;


PYBIND11_MODULE(io, m) {
  m.doc() = "python binding for in/output of particle positions (part of CatAna)";

  // Initialization of random numbers. Once with given seed, once random seed
  m.def("init_random", py::overload_cast<unsigned int> (&init_random), R"pbdoc(
      Sets a specific seed for the random number generator in the io module (used for filtering)

      Parameters:
          seed (unsigned int): the seed for the RNG.
  )pbdoc", py::arg("seed"));
  m.def("init_random", py::overload_cast<>(&init_random), R"pbdoc(
      Sets a random seed for the random number generator in the io module (used for filtering)
  )pbdoc");


  // Parent classes
  py::class_<io::Source> (m, "Source", R"pbdoc(
          (Abstract) parent class for Source implementations.
      )pbdoc")
      .def("reset", &io::Source::reset, R"pbdoc(
           Reset the source

           This reverts the Source to the initial state and reading from it again should produce the same results
      )pbdoc")
      .def("get_point_container", &io::Source::get_point_container, R"pbdoc(
          Load all data into an PointContainer.

          Returns:
              `PointContainer`: A PointContainer with all data
      )pbdoc")
      .def("get_pixelized_point_container", &io::Source::get_pixelized_point_container, R"pbdoc(
          Load all data into a PixelizedPointContainer with given NSide

          Parameters:
              nside (int): HEALPix pixelization resolution, must be a power of 2

          Returns:
              `PixelizedPointContainer`: A pixelized point container with all the data
      )pbdoc", py::arg("nside"));

  py::class_<io::Sink> (m, "Sink", R"pbdoc(
          (Abstract) parent class for Sink implementations

          These are used to write data of type Point to various targets.
      )pbdoc")
      .def("write", py::overload_cast<const PointContainer&>(&io::Sink::write), R"pbdoc(
          write entire point_container directly to sink

          Parameters:
              point_container (PointContainer): source container to write

          Returns:
              int: number of points written. -1 if it failed
      )pbdoc", py::arg("point_container"));

  py::class_<io::Filter> (m, "Filter", R"pbdoc(
          (Abstract) parent class for point-filtering implementations.

      )pbdoc")
      .def("__call__", [&](io::Filter& fi, PointContainer& point_container){
          return fi(point_container);
      }, R"pbdoc(
          Apply filter to a PointContainer

          Runs the filter on all elements in the point_container, removing filtered elements and resizing the
          container.

          Parameters:
              point_container (PointContainer): the container on which the filter is applied.
      )pbdoc", py::arg("point_container"));


  // Add Source classes here
  py::class_<io::PointContainerSource, io::Source>(m, "PointContainerSource", R"pbdoc(
          Read positions from a PointContainer

          Mainly used to add a :class:`PointContainer` to a :class:`FilterStream`.
      )pbdoc")
      .def(py::init<PointContainer&>(), R"pbdoc(
          Constructor from an existing :class:`PointContainer`.

          Parameters:
              point_container (PointContainer): the PointContainer from which to read
      )pbdoc", py::arg("point_container"));

  py::class_<io::GadgetSource, io::Source>(m, "GadgetSource", R"pbdoc(
          Read positions from binary Gadget file

          This reader will only read particles of type 1 from version 2 binary Gadget files.
      )pbdoc")
      .def(py::init<std::string, bool>(), R"pbdoc(
          Constructor from a Gadget filename.

          Parameters:
              filename (str): path to the Gadget file
              verbose (bool): if ``True``, print logging information to stdout (might be in terminal for
                  jupyter notebooks!)
      )pbdoc", py::arg("filename"), py::arg("verbose")=true);

  py::class_<io::TextSource<record_cd>, io::Source>(m, "CartesianTextSource", R"pbdoc(
          Read positions from Text File (cartesian coordinates)

          Only the first 3 columns are read, remaining columns are ignored.

          Note:
              To convert box coordinates to spherical coordinates, the origin is placed at
              ``[0, 0, 0]``. If the coordinates are not centered at 0 they can be moved by using the shift parameter
              (e.g. if the coordinates are within [0, L], use ``shift=-L/2``)
      )pbdoc")
      .def(py::init<std::string, double>(), R"pbdoc(
          Constructor from a textfile filename.

          Parameters:
             filename (str): path to file containing the positions
             shift (float): a coordinate shift that is applied to each axis
      )pbdoc", py::arg("filename"), py::arg("shift")=0.);

  py::class_<io::TextSource<record_sd>, io::Source>(m, "SphericalTextSource", R"pbdoc(
          Read from Text File (spherical coordinates [r, theta, phi])

          Only the first 3 columns are read, remaining columns are ignored.
      )pbdoc")
      .def(py::init<std::string>(), R"pbdoc(
          Constructor from a textfile filename.

          Parameters:
             filename (str): path to file containing the positions
      )pbdoc", py::arg("filename"));

  py::class_<io::TextSource<record_sd_3dex>, io::Source>(m, "SphericalTextSource_3dex", R"pbdoc(
          Read from Text File (spherical coordinates, [theta, phi, r])

          Only the first 3 columns are read, remaining columns are ignored.
      )pbdoc")
      .def(py::init<std::string>(),R"pbdoc(
          Constructor from a textfile filename.

          Parameters:
             filename (str): path to file containing the positions
      )pbdoc",  py::arg("filename"));

  py::class_<io::RawBinarySource<record_sf>, io::Source>(m, "RawBinarySource", R"pbdoc(
          Read from CatAna binary file (float precision, spherical coordinates)
      )pbdoc")
      .def(py::init<std::string, bool>(), R"pbdoc(
          Constructor from a binary file filename.

          Parameters:
              filename (str): path to the binary file
              verbose (bool): if ``True``, print logging information to stdout (might be in terminal for
                  jupyter notebooks!)
      )pbdoc", py::arg("filename"), py::arg("verbose")=true);


  // Add Sink classes here
  py::class_<io::PointContainerSink, io::Sink>(m, "PointContainerSink", R"pbdoc(
          A Sink to write `Point` data into a `PointContainer`
      )pbdoc")
      .def(py::init<PointContainer&>(), R"pbdoc(
          Constructor from an existing `PointContainer`

          Parameters:
              point_container (PointContainer): container to write the points to
      )pbdoc", py::arg("point_container"));

  py::class_<io::PixelizedPointContainerSink, io::Sink>(m, "PixelizedPointContainerSink", R"pbdoc(
          A Sink to write `Point` data into a `PixelizedPointContainer`
      )pbdoc")
      .def(py::init<PixelizedPointContainer&>(), R"pbdoc(
          Construct from an existing `PixelizedPointContainer`

          Parameters:
              pixelized_point_container (PixelizedPointContainer): container to which the Points should be added
      )pbdoc", py::arg("pixelized_point_container"));

  py::class_<io::TextSink<record_cd>, io::Sink>(m, "CartesianTextSink", R"pbdoc(
          A Sink to write `Point` data into a text-file using cartesian coordinates

          Warning:
              The file will be truncated if it already exists
      )pbdoc")
      .def(py::init<std::string, bool>(), R"pbdoc(
          Create Sink text-file

          Parameters:
              filename (str): path to file. Will be overwritten if it already exists.
              verbose (bool): output additional logging information if ``True``.
      )pbdoc", py::arg("filename"), py::arg("verbose")=true);

  py::class_<io::TextSink<record_sd>, io::Sink>(m, "SphericalTextSink", R"pbdoc(
          A Sink to write `Point` data into a text-file using spherical coordinates [r, theta, phi]

          Warning:
              The file will be truncated if it already exists
      )pbdoc")
      .def(py::init<std::string, bool>(), R"pbdoc(
          Create Sink text-file

          Parameters:
              filename (str): path to file. Will be overwritten if it already exists.
              verbose (bool): output additional logging information if ``True``.
      )pbdoc", py::arg("filename"), py::arg("verbose")=true);

  py::class_<io::TextSink<record_sd_3dex>, io::Sink>(m, "SphericalTextSink_3dex", R"pbdoc(
          A Sink to write `Point` data into a text-file using spherical coordinates [theta, phi, r]

          This format is compatible with 3dex

          Warning:
              The file will be truncated if it already exists
      )pbdoc")
      .def(py::init<std::string, bool>(), R"pbdoc(
          Create Sink text-file

          Parameters:
              filename (str): path to file. Will be overwritten if it already exists.
              verbose (bool): output additional logging information if ``True``.
      )pbdoc", py::arg("filename"), py::arg("verbose")=true);

  py::class_<io::RawBinarySink<record_sf>, io::Sink>(m, "RawBinarySink", R"pbdoc(
          A Sink to write `Point` data into a CatAna binary file (float precision, spherical coordinates)
      )pbdoc")
      .def(py::init<std::string, bool, bool>(), R"pbdoc(
          Create Sink binary-file

          Parameters:
              filename (str): path to file. Will be overwritten if it already exists.
              verbose (bool): output additional logging information if ``True``.
              append (bool): if ``True``, Points will be appended to the file. If ``False``, file will be truncated.
      )pbdoc", py::arg("filename"), py::arg("verbose")=true, py::arg("append")=false);


  // Add Filter classes here
  py::class_<io::TophatRadialWindowFunctionFilter, io::Filter>(m, "TophatRadialWindowFunctionFilter", R"pbdoc(
          TopHat Radial WindowFunction Filter
      )pbdoc")
      .def(py::init<double>(), R"pbdoc(
          Constructs a tophat filter with radius ``R0``

          Parameters:
              R0 (float): radius of tophat
      )pbdoc", py::arg("R0"));

  py::class_<io::GaussianRadialWindowFunctionFilter, io::Filter>(m, "GaussianRadialWindowFunctionFilter", R"pbdoc(
          Gaussian Radial WindowFunction Filter

          The acceptance probability is p = exp(-(r/R0)^2)
      )pbdoc")
      .def(py::init<double>(), R"pbdoc(
          Constructs a gaussian radial filter with scale factor R0

          Parameters:
              R0 (float): scale factor
      )pbdoc", py::arg("R0"));

  py::class_<io::GenericRadialWindowFunctionFilter, io::Filter>(m, "GenericRadialWindowFunctionFilter", R"pbdoc(
          Generic Radial WindowFunction Filter

          This filter accepts a generic function f: [0, rmax] -> [0, 1] where the function value is the probability
          that the point with the given radius is accepted.

          Note:
              Using this functionality in Python causes many callback function which are slow. To speed up filtering
              with a generic radial probability function, use the constructor with ``interpolation_points`` in the
              interval [min, max].
      )pbdoc")
      .def(py::init<std::function<double(double)>>(), R"pbdoc(
          Constructor from a python callable

          This will cause callbacks which can be very slow for a large `Source`!

          Parameters:
              function (callable): a function returning the acceptance probability for a given radius
      )pbdoc", py::arg("function"))
      .def(py::init<std::function<double(double)>, size_t, double, double>(), R"pbdoc(
          Constructor from a python callable, with interpolation

          Parameters:
              function (callable): a function returning the acceptance probability for a given radius. should be
                  sufficiently smooth!
              interpolation_points (int): number of sampling points which will be equidistantly distributed between
                  [min, max]
              min (float): lower-bound for interpolation range, should be smaller or equal to the shortest radius to
                  which the filter is applied
              max (float): upper-bound for interpolation range, should be larger or equal to the longest radius to
                  which the filter is applied
      )pbdoc", py::arg("function"), py::arg("interpolation_points"), py::arg("min"), py::arg("max"));;

  py::class_<io::AngularMaskFilter, io::Filter>(m, "AngularMaskFilter", R"pbdoc(
          HEALPix Angular Mask Filter

          Uses a HEALPix map, where each pixel should be either 1 or 0, where 1 corresponds to keep the point if its
          angular coordinates are within the pixel and 0 will remove it.
      )pbdoc")
      .def(py::init<std::string>(), R"pbdoc(
Constructor from a HEALPix FITS file

Parameters:
    mask_file (str): path to the HEALPix mask map.
      )pbdoc", py::arg("mask_file"))
      .def(py::init<const Eigen::ArrayXf&>(), R"pbdoc(
Constructor from a HEALPix map array

Parameters:
    map (numpy.ndarray[float32]): valid HEALPix map, in RING format, containing only 1 and 0 pixel values
      )pbdoc", py::arg("map"));


  // FilterStream
  py::class_<io::FilterStream>(m, "FilterStream", R"pbdoc(
          A Stream which connects a `Source` to a `Sink` and applies filters (`Filter`).

          The FilterStream class is used to move points (`Point`) from the source to the sink while applying (multiple) filters
          and buffering the stream to speed up the transfer. Sources, Sinks and Filters can be any objects derrived from
          the `Source` , `Sink` and `Filter` parent classes.
      )pbdoc")
      .def(py::init<io::Source*, io::Sink*, size_t, bool>(), R"pbdoc(
          Constructor

          Parameters:
              source (Source): from where the points will be read
              sink (Sink): to where the filtered points are stored
              buffer_size (int): size of the buffer (in number of points). Faster if larger, but consumes more memroy.
              verbose (bool): if ``True``, print additional logging information to stdout
      )pbdoc", py::arg("source"), py::arg("sink"), py::arg("buffer_size")=1000000, py::arg("verbose")=true)
//      py::keep_alive<1, 2>(), py::keep_alive<1, 3>())  TODO: add keep_alive back in (removed because of SIGSEGV)
      .def("add_filter", &io::FilterStream::add_filter, R"pbdoc(
          Add a Filter to the pipeline.

          Filters will be applied sequentially in the order that they were added to the FilterStream.

          Parameters:
              filter (Filter): A radial or angular Filter instance
      )pbdoc", py::arg("filter"), py::keep_alive<1, 2>())
      .def("set_source", &io::FilterStream::set_source, R"pbdoc(
          Reset the source to a new source

          Can be used to read from multiple sources to the same sink. To do that, use the ``run_totemp`` function with
          ``append=True`` instead of the `run` method (for all sources). Then call the ``run_fromtemp`` function.

          Parameters:
              source (Source): a source instance
      )pbdoc") //, py::arg("source"), py::keep_alive<1, 2>())  // TODO: add keep_alive back in
      .def("run", &io::FilterStream::run, R"pbdoc(
          Run the pipeline

          If the number of points should be reduced, set ``subsample_size`` to the desired value. If
          ``subsample_size>0`` the process will turn into a 2-step process, where in the first step all the filters will
          be applied and the filtered points will be stored in a temporary file. In the second step the remaining points
          in the temporary file will be randomly subsampled and stored to the sink.

          The 2 steps can also be executed manually, using the `run_totemp` and `run_fromtemp` functions.

          Parameters:
              subsample_size (int): number of points to randomly subsample. Set ``subsample_size=0`` to turn off
                  subsampling.
              temp_filename (str): path to the temporary file (used only for subsampling)

          Returns:
              int: number of points written to the sink
      )pbdoc", py::arg("subsample_size")=0, py::arg("temp_filename")="tmp.bin")
      .def("run_totemp", &io::FilterStream::run_totemp, R"pbdoc(
          Run first intermediate step: write source to temporary file, apply filters

          Parameters:
              temp_filename (str): path to the temporary file
              append (bool): if ``True``, points will be appended to the end of the file. Else the temporary file will
                  be overwritten.

          Returns:
              int: number of points written to the temporary file
      )pbdoc", py::arg("temp_filename")="tmp.bin", py::arg("append")=true)
      .def("run_fromtemp", &io::FilterStream::run_fromtemp, R"pbdoc(
          Run second intermediate step: write temporary file to sink (with subsampling)

          Parameters:
              temp_filename (str): path to the temporary file (from previous step)
              subsample_size (int): number of points to randomly subsample. Set to 0 to turn off subsampling
              remove_temp (bool): If ``True`` the temporary file will be deleted once it is read.

          Returns:
              int: number of points written to the sink
      )pbdoc", py::arg("temp_filename")="tmp.bin", py::arg("subsample_size")=0, py::arg("remove_temp")=true);
};