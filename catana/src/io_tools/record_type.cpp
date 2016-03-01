//
// Created by Michael Bühlmann on 08/02/16.
//

#ifndef CATANA_APP_RECORD_TYPE_CPP_HPP
#define CATANA_APP_RECORD_TYPE_CPP_HPP

#include "catana/io_tools/record_types.hpp"
#include <cstdio>

namespace catana { namespace io {

    ///////////////////////////////////////////////////////////////////////////////
    // Type conversion for HDF5
    template<typename T>
    struct get_hdf5_data_type {
        static hid_t type()
        {
                static_assert(sizeof(T)==-1, "Unknown HDF5 data type (add it to record_type.cpp)");
                return -1;
        }
    };


    template<>
    struct get_hdf5_data_type<float> {
        static hid_t type() { return H5T_NATIVE_FLOAT; }
    };


    template<>
    struct get_hdf5_data_type<double> {
        static hid_t type() { return H5T_NATIVE_DOUBLE; }
    };
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////
    // Cartesian record
    template<class FLOAT_TYPE>
    const hid_t CartesianRecord<FLOAT_TYPE>::field_types[] = {
            get_hdf5_data_type<FLOAT_TYPE>::type(),
            get_hdf5_data_type<FLOAT_TYPE>::type(),
            get_hdf5_data_type<FLOAT_TYPE>::type()
    };

    template<class FLOAT_TYPE>
    char const* CartesianRecord<FLOAT_TYPE>::field_names[] = {
            "pos_x",
            "pos_y",
            "pos_z"
    };

    template<class FLOAT_TYPE>
    const size_t CartesianRecord<FLOAT_TYPE>::field_offsets[] = {
            0,
            sizeof(pos_x),
            2*sizeof(pos_x)
    };

    template<class FLOAT_TYPE>
    const size_t CartesianRecord<FLOAT_TYPE>::field_sizes[] = {
            sizeof(pos_x),
            sizeof(pos_y),
            sizeof(pos_z)
    };

    template<class FLOAT_TYPE>
    const char CartesianRecord<FLOAT_TYPE>::title[] = "Particle positions(cartesian)";

    template<class FLOAT_TYPE>
    std::ostream& operator<<(std::ostream& os, const CartesianRecord<FLOAT_TYPE>& rec)
    {
            os << rec.pos_x << " " << rec.pos_y << " " << rec.pos_z;
            return os;
    }
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////
    // SPHERICAL RECORD BASE
    template<class FLOAT_TYPE>
    SphericalRecordBase<FLOAT_TYPE>::SphericalRecordBase()
            :pos_r(0), pos_theta(0), pos_phi(0) { }

    template<class FLOAT_TYPE>
    SphericalRecordBase<FLOAT_TYPE>::SphericalRecordBase(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi)
            :pos_r(r), pos_theta(theta), pos_phi(phi) { }

    template<class FLOAT_TYPE>
    SphericalRecordBase<FLOAT_TYPE>::SphericalRecordBase(const Object& object, const double& box_size, const double& hubble_param)
            :pos_r(object.r*hubble_param), pos_theta(object.p.theta), pos_phi(object.p.phi) { }

    template<class FLOAT_TYPE>
    Object SphericalRecordBase<FLOAT_TYPE>::object(const double& box_size, const double& hubble_param)
    {
        return object_from_spherical_position(pos_r, pos_theta, pos_phi, hubble_param);
    }

    template<class FLOAT_TYPE>
    const hid_t SphericalRecordBase<FLOAT_TYPE>::field_types[] = {
            get_hdf5_data_type<FLOAT_TYPE>::type(),
            get_hdf5_data_type<FLOAT_TYPE>::type(),
            get_hdf5_data_type<FLOAT_TYPE>::type()
    };

    template<class FLOAT_TYPE>
    char const* SphericalRecordBase<FLOAT_TYPE>::field_names[] = {
            "pos_r",
            "pos_theta",
            "pos_phi"
    };

    template<class FLOAT_TYPE>
    const size_t SphericalRecordBase<FLOAT_TYPE>::field_offsets[] = {
            0,
            sizeof(pos_r),
            2*sizeof(pos_r)
    };

    template<class FLOAT_TYPE>
    const size_t SphericalRecordBase<FLOAT_TYPE>::field_sizes[] = {
            sizeof(pos_r),
            sizeof(pos_theta),
            sizeof(pos_phi)
    };

    template<class FLOAT_TYPE>
    const char SphericalRecordBase<FLOAT_TYPE>::title[] = "Particle positions(cartesian)";

    ///////////////////////////////////////////////////////////////////////////////
    // SPHERICAL RECORD

    template<class FLOAT_TYPE, SphericalTextFormat FMT>
    SphericalRecord<FLOAT_TYPE, FMT>::SphericalRecord()
            :SphericalRecordBase<FLOAT_TYPE>() { }

    template<class FLOAT_TYPE, SphericalTextFormat FMT>
    SphericalRecord<FLOAT_TYPE, FMT>::SphericalRecord(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi)
            :SphericalRecordBase<FLOAT_TYPE>(r, theta, phi) { }

    template<class FLOAT_TYPE, SphericalTextFormat FMT>
    SphericalRecord<FLOAT_TYPE, FMT>::SphericalRecord(const Object& object, const double& box_size, const double& hubble_param)
            :SphericalRecordBase<FLOAT_TYPE>(object, box_size, hubble_param) { }

    template<class FLOAT_TYPE>
    SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>::SphericalRecord()
            :SphericalRecordBase<FLOAT_TYPE>() { }

    template<class FLOAT_TYPE>
    SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>::SphericalRecord(FLOAT_TYPE phi, FLOAT_TYPE theta, FLOAT_TYPE r)
            :SphericalRecordBase<FLOAT_TYPE>(r, theta, phi) { }

    template<class FLOAT_TYPE>
    SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>::SphericalRecord(const Object& object, const double& box_size, const double& hubble_param)
            :SphericalRecordBase<FLOAT_TYPE>(object, box_size, hubble_param) { }

    template<class FLOAT_TYPE, SphericalTextFormat FMT>
    std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, FMT>& rec)
    {
            os << rec.pos_r << " " << rec.pos_theta << " " << rec.pos_phi;
            return os;
    }

    template<class FLOAT_TYPE>
    std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>& rec)
    {
        char buffer[60];
        sprintf(buffer, " %18.14F %18.14F %18.12F", rec.pos_phi, rec.pos_theta, rec.pos_r);
        return os << buffer;
    }
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    // IMPLICIT INSTANTIATION (add needed cases)
    template class CartesianRecord<float>;
    template class CartesianRecord<double>;
    template class SphericalRecordBase<float>;
    template class SphericalRecordBase<double>;
    template class SphericalRecord<float, SphericalTextFormat::RTHETAPHI>;
    template class SphericalRecord<double,SphericalTextFormat::RTHETAPHI>;
    template class SphericalRecord<float, SphericalTextFormat::THREEDEX>;
    template class SphericalRecord<double,SphericalTextFormat::THREEDEX>;
    template std::ostream& operator<<<float> (std::ostream&, const CartesianRecord<float>&);
    template std::ostream& operator<<<double>(std::ostream&, const CartesianRecord<double>&);
    template std::ostream& operator<<<float,  SphericalTextFormat::RTHETAPHI>(std::ostream&, const SphericalRecord<float, SphericalTextFormat::RTHETAPHI>&);
    template std::ostream& operator<<<double, SphericalTextFormat::RTHETAPHI>(std::ostream&, const SphericalRecord<double, SphericalTextFormat::RTHETAPHI>&);
    template std::ostream& operator<<<float> (std::ostream&, const SphericalRecord<float, SphericalTextFormat::THREEDEX>&);
    template std::ostream& operator<<<double> (std::ostream&, const SphericalRecord<double, SphericalTextFormat::THREEDEX>&);
}}

#endif //CATANA_APP_RECORD_TYPE_CPP_HPP