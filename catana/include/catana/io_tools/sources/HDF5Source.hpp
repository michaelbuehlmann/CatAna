//
// Created by Michael Bühlmann on 03/02/16.
//

#ifndef CATANA_APP_READ_HDF5_HPP
#define CATANA_APP_READ_HDF5_HPP


#include "../Source.hpp"
#include "../record_types.hpp"

#include <string>
#include <vector>
#include <cassert>



//! Specify size of chuncks read at once
static const size_t read_chunck = 100000;


//! Read all data from HDF5 table filename/dataset_name.
/*
 * Template parameter defines HDF5 record layout. See hdf5_record_types.hpp
 * dataset_name is absolute path in the HDF5 file to the table to read
 */
template<class RecordType>
ObjectContainer read_hdf5_positions(const std::string& filename, const std::string& dataset_name, double hubble_param,
        double box_size, bool verbose);


//! A source to read from HDF5 table filename/dataset_name.
/*
 * Template parameter defines HDF5 record layout. See hdf5_record_types.hpp
 * dataset_name is absolute path in the HDF5 file to the table to read
 */
template<class RecordType>
class HDF5Source : Source {
public:
    typedef RecordType record_t;
    HDF5Source(
            const std::string& filename, const std::string& dataset_name,
            double hubble_parameter=1, double box_size=0,
            bool verbose=false);

    ~HDF5Source();

    // Not copyable, not assignable
    HDF5Source(const HDF5Source&) = delete;
    HDF5Source& operator=(HDF5Source const&) = delete;

    //!Try to read n objects from table and store in ObjectIterator
    /*
     * ObjectIterator must be a forward iterator.
     * (ObjectIterator, ObjectIterator+n) must be a valid range.
     * Returns number of objects put into object_s. -1 if EOF.
    */

    long long int read(ObjectContainer::iterator write_iterator, size_t n) override;
    long long int read(Object* write_iterator, size_t n) override;

    size_t get_nobjects() override;

private:
    template<class ObjectIterator>
    long long int read_template(ObjectIterator write_iterator, size_t n);
private:
    std::string filename;
    std::string dataset_name;
    hid_t file_id;  // ID of opened file
    RecordType temp_data[read_chunck];  // Buffer Storage

    hsize_t nrecords;
    hsize_t current_record;

    // Field attributes
    std::vector<size_t> field_offsets;
    std::vector<size_t> field_sizes;
    std::vector<size_t> type_sizes;

    double box_size;
    double hubble_parameter;
    bool verbose;
};

#endif //CATANA_APP_READ_HDF5_HPP
