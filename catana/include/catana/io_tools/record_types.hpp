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

#include "../types.hpp"
#include "hdf5.h"

namespace catana { namespace io {

    template<class FLOAT_TYPE>
    struct CartesianRecord {
        typedef FLOAT_TYPE float_t;
        FLOAT_TYPE pos_x;
        FLOAT_TYPE pos_y;
        FLOAT_TYPE pos_z;

        CartesianRecord()
                :pos_x(0), pos_y(0), pos_z(0) { }

        CartesianRecord(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z)
                :pos_x(x), pos_y(y), pos_z(z) { }

        CartesianRecord(const Object& object, const double& box_size, const double& hubble_param)
        {
            pos_x = pos_y = pos_z = object.r*hubble_param;
            vec3 unit_vec(object.p.to_vec3());
            pos_x *= unit_vec.x;
            pos_y *= unit_vec.y;
            pos_z *= unit_vec.z;
            pos_x += 0.5*box_size;
            pos_y += 0.5*box_size;
            pos_z += 0.5*box_size;
        }

        Object object(const double& box_size, const double& hubble_param)
        {
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

    enum class SphericalTextFormat{
        RTHETAPHI,
        THREEDEX
    };

    template<class FLOAT_TYPE>
    struct SphericalRecordBase {
        typedef FLOAT_TYPE float_t;
        FLOAT_TYPE pos_r;
        FLOAT_TYPE pos_theta;
        FLOAT_TYPE pos_phi;

        SphericalRecordBase();
        SphericalRecordBase(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi);
        SphericalRecordBase(const Object& object, const double& box_size, const double& hubble_param);
        Object object(const double& box_size, const double& hubble_param);

        // STUFF FOR HDF5 OUTPUT (defined in record_types.cpp)
        static const unsigned int nfields = 3;
        static char const* field_names[];
        static const hid_t field_types[];
        static const size_t field_offsets[];
        static const size_t field_sizes[];
        static const char title[];
    };

    template<class FLOAT_TYPE, SphericalTextFormat FMT = SphericalTextFormat::RTHETAPHI>
    struct SphericalRecord : public SphericalRecordBase<FLOAT_TYPE>{
        SphericalRecord();
        SphericalRecord(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi);
        SphericalRecord(const Object& object, const double& box_size, const double& hubble_param);
    };

    template<class FLOAT_TYPE>
    struct SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX> : public SphericalRecordBase<FLOAT_TYPE>{
        SphericalRecord();
        SphericalRecord(FLOAT_TYPE theta, FLOAT_TYPE phi, FLOAT_TYPE r);
        SphericalRecord(const Object& object, const double& box_size, const double& hubble_param);
    };

    // Only supports reading
    struct HDF5_PandasCartesianRecord {
        int64 index;
        CartesianRecord<double> cart;

        Object object(const double& box_size, const double& hubble_param)
        {
            return cart.object(box_size, hubble_param);
        }
    };

    // OUT STREAM DECLARATIONS
    template<class FLOAT_TYPE>
    std::ostream& operator<<(std::ostream& os, const CartesianRecord<FLOAT_TYPE>& rec);

    template<class FLOAT_TYPE, SphericalTextFormat FMT>
    std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, FMT>& rec);

    template<class FLOAT_TYPE>
    std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>& rec);

}}
#endif //CATANA_APP_HDF5_RECORD_TYPES_HPP
