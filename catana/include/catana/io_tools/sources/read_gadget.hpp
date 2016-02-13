//
// Created by Michael Bühlmann on 02/02/16.
//

#ifndef CATANA_APP_READ_GADGET_HPP
#define CATANA_APP_READ_GADGET_HPP


#include "catana/io_tools/Source.hpp"
#include <string>

#include <iosfwd>
#include <fstream>

//! Read all objects from filename (and associated files) and output ObjectContainer.
ObjectContainer read_gadget_halo_positions(const std::string& filename, bool verbose=true);

//! A source which can be read sequentially
class GadgetSource : public Source{
public:
    GadgetSource(std::string filename, bool verbose=false);
    // TODO: Destructor closes file

    //!Returns number of objects put into object_s. -1 if EOF.
    // Implemented are Iterators of type ObjectContainer::iterator and Object*

    long long int read(ObjectContainer::iterator write_iterator, size_t n) override;
    long long int read(Object* write_iterator, size_t n) override;

    //!Total number of objects which can be read
    size_t get_nobjects() override;

    static const int skipsize;
private:
    template<class ObjectIterator>
    long long int read_template(ObjectIterator write_iterator, size_t n);

    void load_file(std::string fname, bool initial_file=false);
    void load_file(unsigned int filenum);
private:
    std::string filename;
    unsigned int files;
    unsigned int current_file;
    size_t n_objects;
    size_t n_objects_file;
    size_t current_object;
    size_t current_object_file;

    double box_size;
    double hubble_parameter;

    std::ifstream fd;

    bool verbose;
};

#endif //CATANA_APP_READ_GADGET_HPP
