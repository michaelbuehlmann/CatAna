//
// Created by Michael Bühlmann on 11/02/16.
//

#include <catana/types.hpp>
#include <catana/io.hpp>

#include <tuple>
#include <array>
#include <algorithm>
#include <string>
#include <functional>
#include <random>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace catana;

// Which records will we use for HDF5 files?
using record_cf = io::CartesianRecord<float>;
using record_cd = io::CartesianRecord<double>;
using record_sf = io::SphericalRecord<float>;
using record_sd = io::SphericalRecord<double>;
using record_sd_3dex = io::SphericalRecord<double, io::SphericalTextFormat::THREEDEX>;

// Provide a random number generator
std::random_device rand_dev;
std::mt19937 rng;


PYBIND11_PLUGIN(io) {
    py::module m("io", "python binding for in/output of particle positions (part of CatAna)");

    // Initialization of random numbers. Once with given seed, once random seed
    m.def("init_random", [&](unsigned int seed){
        rng.seed(seed); },
        py::arg("seed"));
    m.def("init_random", [&](){
        std::array<int, 624> seed_data;
        std::generate(seed_data.begin(), seed_data.end(), std::ref(rand_dev));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        rng.seed(seq); });


    // Parent classes
    py::class_<io::Source> (m, "Source")
            .def("reset", &io::Source::reset)
            .def("get_objectcontainer", &io::Source::get_objectcontainer)
            .def("get_pixobjectcontainer", &io::Source::get_pixobjectcontainer, py::arg("nside"));
    py::class_<io::Sink> (m, "Sink");
    py::class_<io::Filter> (m, "Filter")
            .def("__call__", [&](io::Filter& fi, ObjectContainer& object_container){
                return fi(object_container);
            });

    // Add Source classes here
  py::class_<io::ObjectContainerSource, io::Source>(m, "ObjectContainerSource",
                                                    "Read positions from ObjectContainer")
            .def(py::init<ObjectContainer&>());
  py::class_<io::GadgetSource, io::Source>(m, "GadgetSource",
                                           "Read positions from binary Gadget file")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
  py::class_<io::TextSource<record_cd>, io::Source>(m, "TextSource_cartesian",
                                                    "Read from Text File (cartesian coordinates), first 3 columns")
            .def(py::init<std::string, double, double>(),
                    py::arg("filename"), py::arg("hubble_param")=1., py::arg("box_size")=0.);
  py::class_<io::TextSource<record_sd>, io::Source>(m, "TextSource_spherical",
                                                    "Read from Text File (spherical coordinates [r, theta, phi]), first 3 columns")
            .def(py::init<std::string, double, double>(),
                    py::arg("filename"), py::arg("hubble_param")=1., py::arg("box_size")=0.);
  py::class_<io::TextSource<record_sd_3dex>, io::Source>(m, "TextSource_spherical_3dex",
                                                         "Read from Text File (spherical coordinates, [theta, phi, r]), first 3 columns")
            .def(py::init<std::string, double, double>(),
                    py::arg("filename"), py::arg("hubble_param")=1., py::arg("box_size")=0.);
  py::class_<io::RawBinarySource<record_sf>, io::Source>(m, "RawBinarySource_float",
                                                         "Read from CatAna binary file (float precision, spherical coordinates)")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);

    // Add Sink classes here
  py::class_<io::ObjectContainerSink, io::Sink>(m, "ObjectContainerSink",
                                                "Write data into ObjectContainer")
            .def(py::init<ObjectContainer&>());
  py::class_<io::PixelizedObjectContainerSink, io::Sink>(m, "PixelizedObjectContainerSink",
                                                         "Write data into PixelizedObjectContainer")
            .def(py::init<PixelizedObjectContainer&>());
  py::class_<io::TextSink<record_cd>, io::Sink>(m, "TextSink_cartesian",
                                                "Write to Text File (cartesian)")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
  py::class_<io::TextSink<record_sd>, io::Sink>(m, "TextSink_spherical",
                                                "Write to Text File (spherical, [r, theta, phi])")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
  py::class_<io::TextSink<record_sd_3dex>, io::Sink>(m, "TextSink_spherical_3dex",
                                                     "Write to Text File (spherical, [theta, phi, r] 3dex format)")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
  py::class_<io::RawBinarySink<record_sf>, io::Sink>(m, "RawBinarySink_float",
                                                     "Write to CatAna binary file (float precision, spherical coordinates)")
            .def(py::init<std::string, bool, bool>(),
            py::arg("filename"), py::arg("verbose")=true, py::arg("append")=false);

    // Add Filter classes here
  py::class_<io::TophatRadialWindowFunctionFilter, io::Filter>(m, "TophatRadialWindowFunctionFilter",
                                                               "TopHat Radial WindowFunction Filter")
            .def(py::init<double>(), py::arg("R0"));
  py::class_<io::GaussianRadialWindowFunctionFilter, io::Filter>(m, "GaussianRadialWindowFunctionFilter"
            "Gaussian Radial WindowFunction Filter")
            .def(py::init<double>(), py::arg("R0"));
  py::class_<io::GenericRadialWindowFunctionFilter, io::Filter>(m, "GenericRadialWindowFunctionFilter"
            "Radial WindowFunction Filter")
            .def(py::init<std::function<double(double)>>(), py::arg("function"))
            .def(py::init<std::function<double(double)>, size_t, double, double>(),
                    py::arg("function"), py::arg("interpolation_points"), py::arg("min"), py::arg("max"));;
  py::class_<io::AngularMaskFilter, io::Filter>(m, "AngularMaskFilter",
                                                "Healpix Angular Mask Filter. 1->keep, 0->delete")
            .def(py::init<std::string>(), py::arg("mask_file"));

    // FilterStream
    py::class_<io::FilterStream>(m, "FilterStream", "A Stream which needs a source and a sink. Can add Filters")
            .def(py::init<io::Source*, io::Sink*, size_t, bool>(),
                    py::arg("source"), py::arg("sink"), py::arg("buffer_size")=1000000,
                    py::arg("verbose")=true)
            .def("add_filter", &io::FilterStream::add_filter, "Add a Filter to process", py::arg("filter"))
            .def("set_source", &io::FilterStream::set_source, "Set source to new source", py::arg("source"))
            .def("run", &io::FilterStream::run, "Run the pipeline",
                    py::arg("subsample_size"), py::arg("temp_filename")="tmp.bin")
            .def("run_totemp", &io::FilterStream::run_totemp, "Run intermediate steps manually: write source to temporary file, apply filters",
                    py::arg("temp_filename")="tmp.bin", py::arg("append")=true)
            .def("run_fromtemp", &io::FilterStream::run_fromtemp, "Run intermediate steps manually: write temporary file to sink (with subsampling)",
                    py::arg("temp_filename")="tmp.bin", py::arg("subsample_size")=0, py::arg("remove_temp")=true);

    return m.ptr();
};