//
// Created by Michael Bühlmann on 11/02/16.
//

#include <catana/types.hpp>
#include <catana/iotools.hpp>

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


PYBIND11_PLUGIN(io_core) {
    py::module m("io_core", "python binding for in/output of particle positions (part of CatAna)");

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
    py::class_<io::Sink> sink(m, "Sink");
    py::class_<io::Filter> filter(m, "Filter");

    // Add Source classes here
    py::class_<io::GadgetSource>(m, "GadgetSource", py::base<io::Source>(), "Read positions from binary Gadget file")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
    // TEXT
    py::class_<io::TextSource<record_cd>>(m, "TextSource_cartesian", py::base<io::Source>(),
            "Read from Text File (cartesian coordinates), first 3 columns")
            .def(py::init<std::string, double, double>(),
                    py::arg("filename"), py::arg("hubble_param")=1., py::arg("box_size")=0.);
    py::class_<io::TextSource<record_sd>>(m, "TextSource_spherical", py::base<io::Source>(),
            "Read from Text File (spherical coordinates [r, theta, phi]), first 3 columns")
            .def(py::init<std::string, double, double>(),
                    py::arg("filename"), py::arg("hubble_param")=1., py::arg("box_size")=0.);
    py::class_<io::TextSource<record_sd_3dex>>(m, "TextSource_spherical_3dex", py::base<io::Source>(),
            "Read from Text File (spherical coordinates, [theta, phi, r]), first 3 columns")
            .def(py::init<std::string, double, double>(),
                    py::arg("filename"), py::arg("hubble_param")=1., py::arg("box_size")=0.);

    // HDF5
    py::class_<io::HDF5Source<record_cf>>(m, "HDF5Source_cartesian_float", py::base<io::Source>(),
            "Read from HDF5 File (cartesian coordinates, float)")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);
    py::class_<io::HDF5Source<record_cd>>(m, "HDF5Source_cartesian_double", py::base<io::Source>(),
            "Read from HDF5 File (cartesian coordinates, double)")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);
    py::class_<io::HDF5Source<record_sf>>(m, "HDF5Source_spherical_float", py::base<io::Source>(),
            "Read from HDF5 File (spherical coordinates, float)")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);
    py::class_<io::HDF5Source<record_sd>>(m, "HDF5Source_spherical_double", py::base<io::Source>(),
            "Read from HDF5 File (spherical coordinates, double")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);

    // Add Sink classes here
    py::class_<io::HDF5Sink<record_cf>>(m, "HDF5Sink_cartesian_float", sink,
            "Write to HDF5 File (cartesian float)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);
    py::class_<io::HDF5Sink<record_cd>>(m, "HDF5Sink_cartesian_double", sink,
            "Write to HDF5 File (cartesian double)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);
    py::class_<io::HDF5Sink<record_sf>>(m, "HDF5Sink_spherical_float", sink,
            "Write to HDF5 File (spherical float)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);
    py::class_<io::HDF5Sink<record_sd>>(m, "HDF5Sink_spherical_double", sink,
            "Write to HDF5 File (spherical double)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);
    py::class_<io::TextSink<record_cd>>(m, "TextSink_cartesian", sink,
            "Write to Text File (cartesian)")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
    py::class_<io::TextSink<record_sd>>(m, "TextSink_spherical", sink,
            "Write to Text File (spherical, [r, theta, phi])")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
    py::class_<io::TextSink<record_sd_3dex>>(m, "TextSink_spherical_3dex", sink,
            "Write to Text File (spherical, [theta, phi, r] 3dex format)")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);

    // Add Filter classes here
    py::class_<io::TophatRadialWindowFunctionFilter>(m, "TophatRadialWindowFunctionFilter", filter,
            "TopHat Radial WindowFunction Filter")
            .def(py::init<double>(), py::arg("R0"));
    py::class_<io::GaussianRadialWindowFunctionFilter>(m, "GaussianRadialWindowFunctionFilter", filter,
            "Gaussian Radial WindowFunction Filter")
            .def(py::init<double>(), py::arg("R0"));
    py::class_<io::GenericRadialWindowFunctionFilter>(m, "GenericRadialWindowFunctionFilter", filter,
            "Radial WindowFunction Filter")
            .def(py::init<std::function<double(double)>>(), py::arg("function"))
            .def(py::init<std::function<double(double)>, size_t, double, double>(),
                    py::arg("function"), py::arg("interpolation_points"), py::arg("min"), py::arg("max"));;
    py::class_<io::AngularMaskFilter>(m, "AngularMaskFilter", filter,
            "Healpix Angular Mask Filter. 1->keep, 0->delete")
            .def(py::init<std::string>(), py::arg("mask_file"));

    // FilterStream
    py::class_<io::FilterStream>(m, "FilterStream", "A Stream which needs a source and a sink. Can add Filters")
            .def(py::init<io::Source*, io::Sink*, size_t, bool>(),
                    py::arg("source"), py::arg("sink"), py::arg("buffer_size")=1000000, py::arg("verbose")=true)
            .def(py::init<io::Source*, io::Sink*, size_t, size_t, std::string, bool>(),
                    py::arg("source"), py::arg("sink"), py::arg("buffer_size"), py::arg("subset_size"),
                    py::arg("temporary_filename"), py::arg("verbose")=true)
            .def("add_filter", &io::FilterStream::add_filter, "Add a Filter to process", py::arg("filter"))
            .def("run", &io::FilterStream::run, "Run the pipeline");

    return m.ptr();
}