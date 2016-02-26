//
// Created by Michael Bühlmann on 08/02/16.
//

#ifndef CATANA_APP_RECORD_TYPE_CPP_HPP
#define CATANA_APP_RECORD_TYPE_CPP_HPP

#include "catana/io_tools/record_types.hpp"

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
        // SPHERICAL RECORD
        template<class FLOAT_TYPE>
        const hid_t SphericalRecord<FLOAT_TYPE>::field_types[] = {
                get_hdf5_data_type<FLOAT_TYPE>::type(),
                get_hdf5_data_type<FLOAT_TYPE>::type(),
                get_hdf5_data_type<FLOAT_TYPE>::type()
        };

        template<class FLOAT_TYPE>
        char const* SphericalRecord<FLOAT_TYPE>::field_names[] = {
                "pos_r",
                "pos_theta",
                "pos_phi"
        };

        template<class FLOAT_TYPE>
        const size_t SphericalRecord<FLOAT_TYPE>::field_offsets[] = {
                0,
                sizeof(pos_r),
                2*sizeof(pos_r)
        };

        template<class FLOAT_TYPE>
        const size_t SphericalRecord<FLOAT_TYPE>::field_sizes[] = {
                sizeof(pos_r),
                sizeof(pos_theta),
                sizeof(pos_phi)
        };

        template<class FLOAT_TYPE>
        const char SphericalRecord<FLOAT_TYPE>::title[] = "Particle positions(cartesian)";

        template<class FLOAT_TYPE>
        std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE>& rec)
        {
            os << rec.pos_r << " " << rec.pos_theta << " " << rec.pos_phi;
            return os;
        }
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////


        ///////////////////////////////////////////////////////////////////////////////
        // IMPLICIT INSTANTIATION (add needed cases)
        template class CartesianRecord<float>;
        template class CartesianRecord<double>;
        template class SphericalRecord<float>;
        template class SphericalRecord<double>;
        template std::ostream& operator<<<float>(std::ostream&, const CartesianRecord<float>&);
        template std::ostream& operator<<<double>(std::ostream&, const CartesianRecord<double>&);
        template std::ostream& operator<<<float>(std::ostream&, const SphericalRecord<float>&);
        template std::ostream& operator<<<double>(std::ostream&, const SphericalRecord<double>&);
}}

#endif //CATANA_APP_RECORD_TYPE_CPP_HPP