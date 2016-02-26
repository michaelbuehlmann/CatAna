//
// Created by Michael Bühlmann on 13/02/16.
//

#include <catana/io_tools/sinks/HDF5Sink.hpp>

#include <hdf5_hl.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <utility>

namespace catana{ namespace io {

        template<class RecordType>
        HDF5Sink<RecordType>::HDF5Sink(const std::string& filename, const std::string& dataset_name,
                double hubble_parameter, double box_size, bool overwrite_file, bool verbose)
                :filename(filename), dataset_name(dataset_name), box_size(box_size),
                 hubble_parameter(hubble_parameter), verbose(verbose)
        {
            if (!overwrite_file && boost::filesystem::exists(filename)) {
                if (verbose)
                    std::cout << "Opening existing file (fails if dataset already exists)" << std::endl;
                file_id = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            }
            else {
                if (verbose)
                    std::cout << "Creating new file (overwrites existing file)" << std::endl;
                file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
            }
            record_t record;
            herr_t status = H5TBmake_table(
                    record_t::title, file_id, dataset_name.c_str(),
                    record_t::nfields, 0, sizeof(record_t),
                    const_cast<const char**>(&record_t::field_names[0]), record_t::field_offsets,
                    record_t::field_types, 10, 0, 0, &record
            );
            assert(status>=0);
        }

        template<class RecordType>
        HDF5Sink<RecordType>::~HDF5Sink()
        {
            H5Fclose(file_id);
        }

        template<class RecordType>
        template<class ObjectIterator>
        long long int HDF5Sink<RecordType>::write_template(ObjectIterator read_iterator, size_t n)
        {
            RecordType* record_temp = new RecordType[n];
            std::transform(
                    read_iterator, read_iterator+n, record_temp,
                    [&](decltype(*read_iterator) obj) { return record_t(obj, box_size, hubble_parameter); }
            );

            H5TBappend_records(
                    file_id, dataset_name.c_str(), n, sizeof(record_t),
                    record_t::field_offsets, record_t::field_sizes, record_temp
            );
            H5Fflush(file_id, H5F_SCOPE_GLOBAL);

            delete[] record_temp;
            return n;
        }

        template<class RecordType>
        long long int HDF5Sink<RecordType>::write(Object* read_iterator, size_t n)
        {
            return write_template(read_iterator, n);
        }

        template<class RecordType>
        long long int HDF5Sink<RecordType>::write(ObjectContainer::iterator read_iterator, size_t n)
        {
            return write_template(read_iterator, n);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // IMPLICIT INSTANTIATION (add needed cases)
        template
        class HDF5Sink<CartesianRecord<float>>;

        template
        class HDF5Sink<CartesianRecord<double>>;

        template
        class HDF5Sink<SphericalRecord<float>>;

        template
        class HDF5Sink<SphericalRecord<double>>;

}}