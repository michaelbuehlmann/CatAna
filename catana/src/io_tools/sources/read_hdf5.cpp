//
// Created by Michael Bühlmann on 13/02/16.
//

#include <catana/io_tools/sources/read_hdf5.hpp>

#include <hdf5.h>
#include <hdf5_hl.h>
#include <algorithm>
#include <iostream>
#include <cstring>


template <class RecordType>
HDF5Source<RecordType>::HDF5Source(
        const std::string& filename, const std::string& dataset_name,
        double hubble_parameter, double box_size,
        bool verbose)
        : filename(filename), dataset_name(dataset_name), nrecords(0), current_record(0),
          box_size(box_size), hubble_parameter(hubble_parameter), verbose(verbose)
{
    file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    herr_t status;
    // Reading table info
    hsize_t nfields = 0;
    status = H5TBget_table_info(file_id, dataset_name.c_str(), &nfields, &nrecords);
    if (verbose)
        std::cout << "Fields: " << nfields << " Records: " << nrecords << std::endl;

    // Preparing field info storage
    std::vector<char*> field_names_ptr;
    for (int i = 0; i<nfields; ++i) {
        field_names_ptr.push_back(new char[255]);
        std::memset(field_names_ptr[i], 0, 255);
    }
    field_sizes.resize(nfields);
    field_offsets.resize(nfields);
    type_sizes.resize(nfields);
    // Reading field info
    status = H5TBget_field_info(file_id, dataset_name.c_str(),
            &field_names_ptr[0], &field_sizes[0], &field_offsets[0], &type_sizes[0]);
    if (verbose) {
        for (int i = 0; i<nfields; ++i) {
            std::cout << "Field " << i << ":\n"
                    << "\tName: " << field_names_ptr[i] << "\n"
                    << "\tSize: " << field_sizes[i] << "\n"
                    << "\tOffset: " << field_offsets[i] << "\n"
                    << "\tType Size: " << type_sizes[i] << std::endl;
        }
    }
    // Cleaning up
    for(auto cptr: field_names_ptr){
        delete[] cptr;
    }
}

template <class RecordType>
HDF5Source<RecordType>::~HDF5Source(){
    H5Fclose(file_id);}


template<class RecordType> template<class ObjectIterator>
long long int HDF5Source<RecordType>::read_template(ObjectIterator write_iterator, size_t n)
{
    size_t loaded = 0;
    size_t to_load = std::min(n, static_cast<size_t>(nrecords-current_record));

    if(to_load <= 0) return -1;

    herr_t status;
    while(to_load > 0) {
        size_t to_load_step = std::min(to_load, read_chunck);
        status = H5TBread_records(file_id, dataset_name.c_str(),
                current_record, to_load_step,
                sizeof(record_t),
                &field_offsets[0], &field_sizes[0],
                &temp_data[0]);

        if (status<0)
            return -1;

        for (size_t i = 0; i<to_load_step; ++i, ++write_iterator) {
            *write_iterator = temp_data[i].object(box_size, hubble_parameter);
        }

        current_record += to_load_step;
        loaded += to_load_step;
        to_load -= to_load_step;
    }
    return static_cast<long long int>(loaded);
}

template<class RecordType>
long long int HDF5Source<RecordType>::read(ObjectContainer::iterator write_iterator, size_t n) {
    return read_template(write_iterator, n);
}

template<class RecordType>
long long int HDF5Source<RecordType>::read(Object* write_iterator, size_t n) {
    return read_template(write_iterator, n);
}

template<class RecordType>
size_t HDF5Source<RecordType>::get_nobjects() {
    return nrecords;
}

template<class RecordType>
ObjectContainer read_hdf5_positions(const std::string& filename, const std::string& dataset_name, double hubble_param,
        double box_size, bool verbose)
{
    HDF5Source<RecordType> hdf5_source(filename, dataset_name, hubble_param, box_size, verbose);
    ObjectContainer object_container(hdf5_source.get_nobjects());
    auto nrecords = hdf5_source.read(object_container.begin(), hdf5_source.get_nobjects());
    if(nrecords<0)
        throw "Read Error";
    if(nrecords<hdf5_source.get_nobjects())
        object_container.resize(static_cast<size_t>(nrecords));
    return object_container;
}

///////////////////////////////////////////////////////////////////////////////
// IMPLICIT INSTANTIATION (add needed cases)
template class HDF5Source<CartesianRecord<float>>;
template class HDF5Source<CartesianRecord<double>>;
template class HDF5Source<SphericalRecord<float>>;
template class HDF5Source<SphericalRecord<double>>;

template ObjectContainer read_hdf5_positions<CartesianRecord<float>>(const std::string&, const std::string&, double, double, bool);
template ObjectContainer read_hdf5_positions<CartesianRecord<double>>(const std::string&, const std::string&, double, double, bool);;
template ObjectContainer read_hdf5_positions<SphericalRecord<float>>(const std::string&, const std::string&, double, double, bool);;
template ObjectContainer read_hdf5_positions<SphericalRecord<double>>(const std::string&, const std::string&, double, double, bool);;