//
// Created by Michael Bühlmann on 24/02/16.
//

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/complex.h>

#include <Eigen/Dense>

#include <catana/types.hpp>
#include <catana/tools/FunctionInterpolator.hpp>

namespace py = pybind11;
using namespace catana;


PYBIND11_PLUGIN(basictypes)
{
    py::module m("basictypes", "python bindings of basic types used in CatAna");

    // Binding for the "Object" class, with cartesian and spherical output methods
    py::class_<Object>(m, "Object", py::buffer_protocol())
            .def(py::init<>())
            .def(py::init<double, double, double>())
            .def("spherical", [](const Object& obj) {
                return std::make_tuple(obj.r, obj.p.theta, obj.p.phi);
            })
            .def("cartesian", [](const Object& obj) {
                vec3 v = obj.p.to_vec3();
                return std::make_tuple(obj.r*v.x, obj.r*v.y, obj.r*v.z);
            })
            .def_buffer([](Object& object) -> py::buffer_info {
                return py::buffer_info(
                        &object.r,
                        sizeof(double),
                        py::format_descriptor<double>::format(),
                        1,
                        {3},
                        {sizeof(double)}
                );
            });

    // Binding of the ObjectContainer class
    py::class_<ObjectContainer>(m, "ObjectContainer", py::buffer_protocol())
            .def(py::init<>())
            .def("__init__", [](ObjectContainer& oc, py::array_t<double> array, std::string coord_type)
            {
                bool spherical;
                if(coord_type==std::string("cartesian"))
                    spherical=false;
                else if(coord_type==std::string("spherical"))
                    spherical= true;
                else
                    throw std::runtime_error("coord_type either 'cartesian' or 'spherical'");

                py::buffer_info info = array.request();
                if(info.ndim != 2 )
                    throw std::runtime_error("Incompatible number of dimensions of array (needs 2)");
                if(info.shape[1] != 3)
                    throw std::runtime_error("Incompatible number of columns of array (needs 3)");

                new (&oc) ObjectContainer();  //
                double* data_ptr = (double*) info.ptr;  // A pointer to the storage
                double* data_ptr_end = data_ptr + info.shape[0]*3;  // A pointer behind the storage

                if (info.strides[1] == sizeof(double)){  // Correct data ordering
                    for(double* current_ptr = data_ptr; current_ptr != data_ptr_end; current_ptr+=3){
                        if(spherical)
                            oc.add_object(object_from_spherical_position(current_ptr, 1));
                        else
                            oc.add_object(object_from_box_position(current_ptr, 0, 1));
                    }
                } else {  // Wrong data ordering
                    throw std::runtime_error("Data ordering of array is ColumnMajor, but RowMajor needed.");
                }
            })

            .def("__getitem__", [](const ObjectContainer& oc, size_t i)
            {
                return oc[i];
            })

                    // Buffer definition, so that we can call numpy.array(object_container) in python
            .def_buffer([](ObjectContainer& oc) -> py::buffer_info {
                return py::buffer_info(
                        (double*) oc.data(),
                        sizeof(double),
                        py::format_descriptor<double>::format(),
                        2,
                        { oc.size(), 3},
                        { sizeof(double) * 3, sizeof(double)}
                );
            })
            .def("add_object", &ObjectContainer::add_object);

    py::class_<PixelizedObjectContainer>(m, "PixelizedObjectContainer")
            .def(py::init<unsigned int>())
            .def(py::init<unsigned int, ObjectContainer>())
            .def("get_countmap", &PixelizedObjectContainer::get_countmap);

    py::class_<FunctionInterpolator>(m, "FunctionInterpolator")
            .def("__init__", [](FunctionInterpolator& fi, std::function<double(double)> fct,
                    unsigned int interpolation_points, double x_min, double x_max) {
                new (&fi) FunctionInterpolator(fct, interpolation_points, x_min, x_max, false);
            })
            .def("__call__", &FunctionInterpolator::operator())
            .def("__call__", [](FunctionInterpolator& fi, py::array_t<double> x) {
                auto stateful_closure = [&fi](double x){return fi(x);};
                return py::vectorize(stateful_closure)(x);
            });

    return m.ptr();
}