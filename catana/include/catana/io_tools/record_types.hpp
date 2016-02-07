//
// The record classes defined here map out the memory layout of a row in the HDF5 Files
// All classes must define a member function .object(box_size, hubble_param) which returns an Object.
//

#ifndef CATANA_APP_HDF5_RECORD_TYPES_HPP
#define CATANA_APP_HDF5_RECORD_TYPES_HPP

#include "catana/tools/object_types.hpp"

template<class FLOAT_TYPE>
struct CartesianRecord {
    FLOAT_TYPE pos_x;
    FLOAT_TYPE pos_y;
    FLOAT_TYPE pos_z;
    Object object(const double& box_size, const double& hubble_param){
        return object_from_box_position(pos_x, pos_y, pos_z, box_size, hubble_param);
    }
};

template<class FLOAT_TYPE>
struct SphericalRecord {
    FLOAT_TYPE pos_r;
    FLOAT_TYPE pos_theta;
    FLOAT_TYPE pos_phi;
    Object object(const double& box_size, const double& hubble_param){
        return object_from_spherical_position(pos_r, pos_phi, pos_theta, hubble_param);
    }
};

struct HDF5_PandasCartesianRecord{
    int64 index;
    CartesianRecord<double> cart;
    Object object(const double& box_size, const double& hubble_param){
        return cart.object(box_size, hubble_param);
    }
};

#endif //CATANA_APP_HDF5_RECORD_TYPES_HPP
