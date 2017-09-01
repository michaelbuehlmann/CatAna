//
// Created by Michael Bühlmann on 24/02/16.
//

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/complex.h>

#include <Eigen/Dense>

#include <catana/catana.hpp>

namespace py = pybind11;
using namespace catana;


PYBIND11_MODULE(basictypes, m)
{
    m.doc() = "python bindings of basic types used in CatAna";

    // Binding for the "Point" class, with cartesian and spherical output methods
    py::class_<Point>(m, "Point", py::buffer_protocol(), R"pbdoc(
            Representation of a particle in 3-dimensional space in spherical coordinates.

            **Convention**:
               - longitude phi in (0, 2pi)
               - lattitude theta in (0, theta) measured from z-axis.

            (can be converted to numpy array (r, theta, phi) by calling ``np.array(point)``)
        )pbdoc")
        .def(py::init<>(), R"pbdoc(
            Default constructor (0,0,0)
        )pbdoc")
        .def(py::init<double, double, double>(), R"pbdoc(
            Constructor from cartesian coordinates
        )pbdoc", py::arg("x"), py::arg("y"), py::arg("z"))
        .def("spherical", [](const Point& obj) {
            return std::make_tuple(obj.r, obj.p.theta, obj.p.phi);
        }, R"pbdoc(
            get spherical point coordinates

            Returns:
                tuple[float]: spherical coordinates (r, theta, phi)
        )pbdoc")
        .def("cartesian", [](const Point& obj) {
            vec3 v = obj.p.to_vec3();
            return std::make_tuple(obj.r*v.x, obj.r*v.y, obj.r*v.z);
        }, R"pbdoc(
            get cartesian point coordinates

            Returns:
               tuple[float]: cartesian coordinates (x, y, z)
        )pbdoc")
        .def_buffer([](Point& point) -> py::buffer_info {
            return py::buffer_info(
                    &point.r,
                    sizeof(double),
                    py::format_descriptor<double>::format(),
                    1,
                    {3},
                    {sizeof(double)}
            );
        });

    // Binding of the PointContainer class
    py::class_<PointContainer>(m, "PointContainer", py::buffer_protocol(), R"pbdoc(
            Collection of :class:`Point` objects

            (can be converted to numpy array with shape (n_points, 3), where each point is of the form
            (r, theta, phi) by calling ``np.array(point_container)``)
        )pbdoc")
        .def(py::init<>(), R"pbdoc(
Default constructor (empty container)
        )pbdoc")
        .def("__init__", [](PointContainer& oc, py::array_t<double> array, std::string coord_type)
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

            new (&oc) PointContainer();  //
            double* data_ptr = (double*) info.ptr;  // A pointer to the storage
            double* data_ptr_end = data_ptr + info.shape[0]*3;  // A pointer behind the storage

            if (info.strides[1] == sizeof(double)){  // Correct data ordering
                for(double* current_ptr = data_ptr; current_ptr != data_ptr_end; current_ptr+=3){
                    if(spherical)
                        oc.add_point(point_from_spherical_position(current_ptr));
                    else
                        oc.add_point(point_from_box_position(current_ptr, 0));
                }
            } else {  // Wrong data ordering
                throw std::runtime_error("Data ordering of array is ColumnMajor, but RowMajor needed.");
            }
        }, R"pbdoc(
Create a PointContainer from a (numpy) array containing the coordinates

Parameters:
    coordinates (numpy.ndarray[float64]) coordinates: coordinate array of shape (n_points, 3)
    coordinate_type (str): coordinate system, either "cartesian" or "spherical"

Note:
    coordinate array has to be in row-major ordering.
        )pbdoc", py::arg("coordinates"), py::arg("coordinate_type")="spherical")

        .def("__getitem__", [](const PointContainer& oc, size_t i)
        {
          if(i >= oc.size())
            throw pybind11::index_error("index out of bounds!");
          return oc[i];
        })

                // Buffer definition, so that we can call numpy.array(point_container) in python
        .def_buffer([](PointContainer& oc) -> py::buffer_info {
            return py::buffer_info(
                    &(oc.data()->r),
                    sizeof(double),
                    py::format_descriptor<double>::format(),
                    2,
                    std::array<long int, 2>{static_cast<long int>(oc.size()), 3},
                    { sizeof(double) * 3, sizeof(double)}
            );
        })
        .def("add_point", &PointContainer::add_point, R"pbdoc(
            Add a point to the collection

            Parameters:
                point (Point): the point to add
        )pbdoc", py::arg("point"))
        .def("__len__", [](const PointContainer& oc){ return oc.size(); });

    py::class_<PixelizedPointContainer>(m, "PixelizedPointContainer", R"pbdoc(
            Pixelized collection of :class:`Point` objects

            Pixelization occurs along the angular axis according to the HEALPix scheme.
            Radial coordinates remain the same.
        )pbdoc")
        .def(py::init<unsigned int>(), R"pbdoc(
Default constructor (empty container)

Parameters:
    nside (int): NSide resolution parameter of the HEALPix scheme. Must be a power of 2.
        )pbdoc", py::arg("nside"))
        .def(py::init<unsigned int, PointContainer>(), R"pbdoc(
Constructor from a :class:`PointContainer`

Parameters:
    nside (int): NSide resolution parameter of the HEALPix scheme. Must be a power of 2.
    point_container (PointContainer): A container of :class:`Point` objects
        )pbdoc", py::arg("nside"), py::arg("point_container"))
        .def("get_countmap", &PixelizedPointContainer::get_countmap, R"pbdoc(
            Obtain a HEALPix count map which contains the number of points which are stored in each pixel.

            Returns:
                numpy.ndarray[int]: HEALPix map with point number counts

            Note:
                The resulting array can be viewed with healpy.
        )pbdoc");

    py::class_<FunctionInterpolator>(m, "FunctionInterpolator", R"pbdoc(
            A function sampler and interpolator

            Samples the given function with ``interpolation_points`` number of equidistantly distributed points between
            ``x_min`` and ``x_max``. When called at a given position, interpolates linearly from the two closest sample
            points. Note that evaluating the function for values ``x >= x_max`` and ``x < x_min`` raises a
            ValueError.

            The FunctionInterpolator mainly serves to speed up computations since we do not need to do expensive
            Python object calls once it is initialized.
        )pbdoc")
        .def("__init__", [](FunctionInterpolator& fi, std::function<double(double)> fct,
                unsigned int interpolation_points, double x_min, double x_max) {
            new (&fi) FunctionInterpolator(fct, interpolation_points, x_min, x_max, false);
        }, R"pbdoc(
            Construct a FunctionInterpolator from a function

            Parameters:
                function (callable object): the function to be sampled and interpolated
                interpolation points (int): number of points at which the function needs to be sampled
                x_min (float): lower boundary of sample points
                x_max (float): upper boundary of sample points
        )pbdoc", py::arg("function"), py::arg("interpolation_points"), py::arg("x_min"), py::arg("x_max"))
        .def("__call__", &FunctionInterpolator::operator(), R"pbdoc(
Evaluate the FunctionInterpolator at the given value x.

Warning:
    x must be within the boundaries [x_min, x_max) for which the FunctionInterpolator was set up.

Parameters:
    x (float): evaluation point
        )pbdoc", py::arg("x"))
        .def("__call__", [](FunctionInterpolator& fi, py::array_t<double> x) {
            auto stateful_closure = [&fi](double x){return fi(x);};
            return py::vectorize(stateful_closure)(x);
        }, R"pbdoc(
Evaluate the FunctionInterpolator at the given values x.

Warning:
    each element in x must be within the boundaries [x_min, x_max) for which the FunctionInterpolator
    was set up.

Parameters:
    x (numpy.ndarray[float]): evaluation points
        )pbdoc", py::arg("x"));
}