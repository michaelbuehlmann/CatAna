//
// Created by Michael Bühlmann on 02/02/16.
//

#ifndef CATANA_APP_READ_GADGET_HPP
#define CATANA_APP_READ_GADGET_HPP

#include "../Source.hpp"
#include <string>

#include <iosfwd>
#include <fstream>

namespace catana { namespace io {

    //! A Source class for Binary Gadget files.
    /*!
     * While reading, the coordinates will be automatically transformed from Mpc/h -> Mpc and the origin is moved to
     * the center of the box.
     */
    class GadgetSource : public Source {
    public:
        //! Construct Source from filename
        /*!
         * @param filename Data spread across multiple files will be automatically detected and loaded.  Specify any of the files.
         * @param verbose Set to true for additional output to stdout
         */
        GadgetSource(std::string filename, bool verbose = false);

        //! Read next n objects from file(s). Returns number of objects put into object_s. -1 if EOF.
        /*!
         * @param write_iterator iterator of ObjectContainer of Objects. [write_iterator, write_iterator + n]
         * must be a valid range.
         * @param n number of objects to read
         */
        long long int read(ObjectContainer::iterator write_iterator, size_t n) override;

        //! Read next n objects from file(s). Returns number of objects put into object_s. -1 if EOF.
        /*!
         * @param write_iterator pointer to a Object[] C-array of Objects. [write_iterator, write_iterator + n]
         * must be a valid range.
         * @param n number of objects to read
         */
        long long int read(Object* write_iterator, size_t n) override;

        //! Total number of objects which can be read.
        size_t get_nobjects() override;

        //! Resets Source to initial state (everything can be read again)
        void reset() override;

        //! The size of the empty elements between blocks in the Gadget file (usually 4 bytes)
        static const int skipsize;
    private:
        template<class ObjectIterator>
        long long int read_template(ObjectIterator write_iterator, size_t n);

        void load_file(std::string fname, bool initial_file = false);

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

}}
#endif //CATANA_APP_READ_GADGET_HPP