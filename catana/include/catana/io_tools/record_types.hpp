// The record classes defined here map out the memory layout of a row in the HDF5 Files
/*
 * All classes must define a member function  which returns an Object
 * and a constructor from an Object and box_size, hubble_param.
 *
 * Add these members:
 *      CartesianRecord()
 *      CartesianRecord(const Object&, const double& box_size, const double& hubble_param)
 *      Object object(box_size, hubble_param)
 *
 *      static unsigned int nfields
 *      static char const* field_names[]
 *      static const hid_t field_types[]
 *      static const size_t field_offsets[]
 *      static const size_t field_sizes[]
 *      static const char title[]
 */


#ifndef CATANA_APP_HDF5_RECORD_TYPES_HPP
#define CATANA_APP_HDF5_RECORD_TYPES_HPP

#include "catana/tools/object_types.hpp"
#include "hdf5.h"

template<class FLOAT_TYPE>
struct CartesianRecord {
    FLOAT_TYPE pos_x;
    FLOAT_TYPE pos_y;
    FLOAT_TYPE pos_z;
    CartesianRecord(): pos_x(0), pos_y(0), pos_z(0) {}
    CartesianRecord(const Object& object, const double& box_size, const double& hubble_param) {
        pos_x = pos_y = pos_z = object.r*hubble_param;
        vec3 unit_vec(object.p.to_vec3());
        pos_x *= unit_vec.x;
        pos_y *= unit_vec.y;
        pos_z *= unit_vec.z;
        pos_x += 0.5*box_size;
        pos_y += 0.5*box_size;
        pos_z += 0.5*box_size;
    }
    Object object(const double& box_size, const double& hubble_param){
        return object_from_box_position(pos_x, pos_y, pos_z, box_size, hubble_param);
    }

    // STUFF FOR HDF5 OUTPUT (defined in record_types.cpp)
    static const unsigned int nfields = 3;
    static char const* field_names[nfields];
    static const hid_t field_types[nfields];
    static const size_t field_offsets[nfields];
    static const size_t field_sizes[nfields];
    static const char title[];
};

template<class FLOAT_TYPE>
struct SphericalRecord {
    FLOAT_TYPE pos_r;
    FLOAT_TYPE pos_theta;
    FLOAT_TYPE pos_phi;
    SphericalRecord() : pos_r(0), pos_theta(0), pos_phi(0) {}
    SphericalRecord(const Object& object, const double& box_size, const double& hubble_param)
            : pos_r(object.r), pos_theta(object.p.theta), pos_phi(object.p.phi) {}
    Object object(const double& box_size, const double& hubble_param){
        return object_from_spherical_position(pos_r, pos_theta, pos_phi, hubble_param);
    }

    // STUFF FOR HDF5 OUTPUT (defined in record_types.cpp)
    static const unsigned int nfields = 3;
    static char const* field_names[];
    static const hid_t field_types[];
    static const size_t field_offsets[];
    static const size_t field_sizes[];
    static const char title[];
};

// Only supports reading
struct HDF5_PandasCartesianRecord{
    int64 index;
    CartesianRecord<double> cart;
    Object object(const double& box_size, const double& hubble_param){
        return cart.object(box_size, hubble_param);
    }
};

#endif //CATANA_APP_HDF5_RECORD_TYPES_HPP
