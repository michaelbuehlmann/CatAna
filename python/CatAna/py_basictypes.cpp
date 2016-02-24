//
// Created by Michael Bühlmann on 24/02/16.
//

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include <Eigen/Dense>

#include <catana/types.hpp>

namespace py = pybind11;

PYBIND11_PLUGIN(basictypes)
{
    py::module m("basictypes", "python bindings of basic types used in CatAna");

    // Binding Eigen array to numpy
    py::class_<Eigen::ArrayXd>(m, "EigenArrayXd")
            .def_buffer([](Eigen::ArrayXd &a) -> py::buffer_info{
                return py::buffer_info(
                        a.data(),  // pointer to buffer
                        sizeof(double),  // size of scalar
                        py::format_descriptor<double>::value(),  // python format descriptor
                        1,  // number of dimensions
                        {static_cast<unsigned long int>(a.rows())},  // the dimensions
                        {sizeof(double)}  // strides (in bytes)
                );
            });

    // Binding Eigen array to numpy
    py::class_<Eigen::ArrayXi>(m, "EigenArrayXi")
            .def_buffer([](Eigen::ArrayXi &a) -> py::buffer_info{
                return py::buffer_info(
                        a.data(),  // pointer to buffer
                        sizeof(int),  // size of scalar
                        py::format_descriptor<int>::value(),  // python format descriptor
                        1,  // number of dimensions
                        {static_cast<unsigned long int>(a.rows())},  // the dimensions
                        {sizeof(int)}  // strides (in bytes)
                );
            });

    // Binding Eigen array to numpy
    py::class_<Eigen::ArrayXXd>(m, "EigenArrayXXd")
            .def_buffer([](Eigen::ArrayXXd &a) -> py::buffer_info{
                return py::buffer_info(
                        a.data(),  // pointer to buffer
                        sizeof(double),  // size of scalar
                        py::format_descriptor<double>::value(),  // python format descriptor
                        2,  // number of dimensions
                        {static_cast<unsigned long int>(a.rows()), static_cast<unsigned long int>(a.cols())},  // the dimensions
                        {sizeof(double), sizeof(double)*a.rows()}  // strides (in bytes)
                );
            });

    // Binding for the "Object" class, with cartesian and spherical output methods
    py::class_<Object>(m, "Object")
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
                        py::format_descriptor<double>::value(),
                        1,
                        {3},
                        {sizeof(double)}
                );
            });

    // Binding of the ObjectContainer class
    py::class_<ObjectContainer>(m, "ObjectContainer")
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
                        py::format_descriptor<double>::value(),
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

    return m.ptr();
}