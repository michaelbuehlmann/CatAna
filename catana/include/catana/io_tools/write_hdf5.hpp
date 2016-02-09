//
// Created by Michael Bühlmann on 08/02/16.
//

#ifndef CATANA_APP_WRITE_HDF5_HPP_HPP
#define CATANA_APP_WRITE_HDF5_HPP_HPP


#include "catana/tools/object_types.hpp"
#include "record_types.hpp"
#include "hdf5.h"

//! A sink to write to HDF5 table filename/dataset_name.
/*
 * If table already exists, overwrite it.
 *
 * Template parameter defines HDF5 record layout. See hdf5_record_types.hpp
 * dataset_name is absolute path in the HDF5 file to the table to read
 */
template<class RecordType>
class HDF5Sink {
public:
    typedef RecordType record_t;
    HDF5Sink(const std::string& filename, const std::string& dataset_name,
                double hubble_parameter, double box_size, bool overwrite_file=true, bool verbose=true);

    ~HDF5Sink();

    // Not copyable, not assignable
    HDF5Sink(const HDF5Sink&) = delete;
    HDF5Sink& operator=(HDF5Sink const&) = delete;

    //!Try to read n objects from table and store in ObjectIterator
    /*
     * ObjectIterator must be a forward iterator.
     * (ObjectIterator, ObjectIterator+n) must be a valid range.
     * Returns number of objects put into object_s. -1 if EOF.
    */
    template<class ObjectIterator>
    long long int write(ObjectIterator read_iterator, size_t n);

private:
    std::string filename;
    std::string dataset_name;
    hid_t file_id;  // ID of opened file

    double box_size;
    double hubble_parameter;
    bool verbose;
};

#include "write_hdf5.ipp"


#endif //CATANA_APP_WRITE_HDF5_HPP_HPP
