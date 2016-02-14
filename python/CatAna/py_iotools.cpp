//
// Created by Michael Bühlmann on 11/02/16.
//

#include <catana/types.hpp>
#include <catana/iotools.hpp>

#include <tuple>
#include <string>
#include <functional>
#include <random>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;


// Which records will we use for HDF5 files?
using record_cf = CartesianRecord<float>;
using record_cd = CartesianRecord<double>;
using record_sf = SphericalRecord<float>;
using record_sd = SphericalRecord<double>;
// Provide a random number generator
std::mt19937 rng;


PYBIND11_PLUGIN(iotools) {
    py::module m("iotools", "python binding for in/output of particle positions (part of CatAna)");

    py::class_<Object>(m, "Object")
            .def(py::init<>())
            .def(py::init<double, double, double>())
            .def("spherical", [](const Object& obj) {
                return std::make_tuple(obj.r, obj.p.theta, obj.p.phi);
            })
            .def("cartesian", [](const Object& obj) {
                vec3 v = obj.p.to_vec3();
                return std::make_tuple(obj.r*v.x, obj.r*v.y, obj.r*v.z);
            });

    // Parent classes
    py::class_<Source> source(m, "Source");
    py::class_<Sink> sink(m, "Sink");
    py::class_<Filter> filter(m, "Filter");

    // Add Source classes here
    py::class_<GadgetSource>(m, "GadgetSource", source, "Read positions from binary Gadget file")
            .def(py::init<std::string, bool>(),
                    py::arg("filename"), py::arg("verbose")=true);
    py::class_<HDF5Source<record_cf>>(m, "HDF5Source_cartesian_float", source,
            "Read from HDF5 File (cartesian coordinates, float)")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);
    py::class_<HDF5Source<record_cd>>(m, "HDF5Source_cartesian_double", source,
            "Read from HDF5 File (cartesian coordinates, double)")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);
    py::class_<HDF5Source<record_sf>>(m, "HDF5Source_spherical_float", source,
            "Read from HDF5 File (spherical coordinates, float)")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);
    py::class_<HDF5Source<record_sd>>(m, "HDF5Source_spherical_double", source,
            "Read from HDF5 File (spherical coordinates, double")
            .def(py::init<std::string, std::string, double, double, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("verbose")=true);

    // Add Sink classes here
    py::class_<HDF5Sink<record_cf>>(m, "HDF5Sink_cartesian_float", sink,
            "Write to HDF5 File (cartesian float)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);
    py::class_<HDF5Sink<record_cd>>(m, "HDF5Sink_cartesian_double", sink,
            "Write to HDF5 File (cartesian double)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);
    py::class_<HDF5Sink<record_sf>>(m, "HDF5Sink_spherical_float", sink,
            "Write to HDF5 File (spherical float)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);
    py::class_<HDF5Sink<record_sd>>(m, "HDF5Sink_spherical_double", sink,
            "Write to HDF5 File (spherical double)")
            .def(py::init<std::string, std::string, double, double, bool, bool>(),
                    py::arg("filename"), py::arg("tablename"), py::arg("hubble_param")=1., py::arg("box_size")=0.,
                    py::arg("new_file")=true, py::arg("verbose")=true);

    // Add Filter classes here
    py::class_<TophatRadialWindowFunctionFilter>(m, "TophatRadialWindowFunctionFilter", filter,
            "TopHat Radial WindowFunction Filter")
            .def(py::init<double>(), py::arg("R0"));
    py::class_<GaussianRadialWindowFunctionFilter>(m, "GaussianRadialWindowFunctionFilter", filter,
            "Gaussian Radial WindowFunction Filter")
            .def(py::init<double>(), py::arg("R0"));
    py::class_<GenericRadialWindowFunctionFilter>(m, "GenericRadialWindowFunctionFilter", filter,
            "Radial WindowFunction Filter")
            .def(py::init<std::function<double(double)>>(), py::arg("function"))
            .def(py::init<std::function<double(double)>, size_t, double, double>(),
                    py::arg("function"), py::arg("interpolation_points"), py::arg("min"), py::arg("max"));;
    py::class_<AngularMaskFilter>(m, "AngularMaskFilter", filter,
            "Healpix Angular Mask Filter. 1->keep, 0->delete")
            .def(py::init<std::string>(), py::arg("mask_file"));

    // FilterStream
    py::class_<FilterStream>(m, "FilterStream", "A Stream which needs a source and a sink. Can add Filters")
            .def(py::init<Source*, Sink*, size_t, bool>(),
                    py::arg("source"), py::arg("sink"), py::arg("buffer_size")=1000000, py::arg("verbose")=true)
            .def(py::init<Source*, Sink*, size_t, size_t, std::string, bool>(),
                    py::arg("source"), py::arg("sink"), py::arg("buffer_size"), py::arg("subset_size"),
                    py::arg("temporary_filename"), py::arg("verbose")=true)
            .def("add_filter", &FilterStream::add_filter, "Add a Filter to process", py::arg("filter"))
            .def("run", &FilterStream::run, "Run the pipeline");
    return m.ptr();
}