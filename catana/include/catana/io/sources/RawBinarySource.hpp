//
// Created by Michael Bühlmann on 14/02/16.
//

#ifndef CATANA_READ_RAWBINARY_HPP_HPP
#define CATANA_READ_RAWBINARY_HPP_HPP

#include "../record_types.hpp"
#include "../Source.hpp"
#include <string>
#include <fstream>

namespace catana { namespace io {

    //! A Source class for raw binary files. The Objects are assumed to be stored in the RecordType Format.
    /*!
     * @tparam RecordType A class defined in the record_types.hpp file. Determines the storage order in the binary
     * file.
     */
    template<class RecordType>
    class RawBinarySource : public Source {
    public:
        typedef RecordType record_t;

        //! Construct from filename.
        RawBinarySource(std::string filename, bool verbose = true);

        //! Read next n objects from file. Returns number of objects put into object_s. -1 if EOF.
        /*!
         * @param write_iterator iterator of ObjectContainer of objects. [write_iterator, write_iterator + n]
         * must be a valid range.
         * @param n number of objects to read
         */
        virtual long long int read(ObjectContainer::iterator write_iterator, size_t n) override;

        //! Read next n objects from. Returns number of objects put into object_s. -1 if EOF.
        /*!
         * @param write_iterator pointer to a Object[] C-array of objects. [write_iterator, write_iterator + n]
         * must be a valid range.
         * @param n number of objects to read
         */
        virtual long long int read(Object* write_iterator, size_t n) override;

        //! Total number of objects which can be read (i.e. size of object_container)
        size_t get_nobjects() override;

        //! Resets Source to initial state (everything can be read again)
        void reset() override;

        // Non copyable and assignable
        RawBinarySource(RawBinarySource const&) = delete;
        RawBinarySource& operator=(RawBinarySource const&) = delete;

    private:
        template<class ObjectIterator>
        long long int read_template(ObjectIterator write_iterator, size_t n);

        std::string filename;
        std::ifstream fd;
        size_t ntot;
        size_t current_pos;
        bool verbose;
    };

}}
#endif //CATANA_READ_RAWBINARY_HPP_HPP
