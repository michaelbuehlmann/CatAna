//
// Created by Michael Bühlmann on 26/02/16.
//

#ifndef CATANA_TEXTSINK_HPP
#define CATANA_TEXTSINK_HPP

#include "../record_types.hpp"
#include "../Sink.hpp"
#include <string>
#include <fstream>

namespace catana { namespace io {

    //! A Sink class for text files. The Objects will be stored in the RecordType Format.
    /*!
     * @tparam RecordType A class defined in the record_types.hpp file. Determines the storage order in the binary
     * file.
     */
    template<class RecordType>
    class TextSink : public Sink {
    public:
        typedef RecordType record_t;

        //! Construct from filename. File with "filename" will be created
        TextSink(std::string filename, bool verbose = true);

        //! write objects within [read_iterator, read_iterator+n) to object_container.
        /*!
         * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
         * @param n number of objects to read from read_iterator and write to sink
         * @return number of objects written. -1 if failed
         */
        virtual long long int write(ObjectContainer::iterator read_iterator, size_t n) override;

        //! write objects within [read_iterator, read_iterator+n) to object_container.
        /*!
         * @param read_iterator [read_iterator, read_iterator+n) must be a valid range of objects
         * @param n number of objects to read from read_iterator and write to sink
         * @return number of objects written. -1 if failed
         */
        virtual long long int write(Object* read_iterator, size_t n) override;

        //! Close file (can no longer write)
        void close();

        // Non copyable and assignable
        TextSink(TextSink const&) = delete;
        TextSink& operator=(TextSink const&) = delete;

    private:
        template<class ObjectIterator>
        long long int write_template(ObjectIterator read_iterator, size_t n);

    private:
        std::string filename;
        std::ofstream fd;
        bool verbose;
    };

}}
#endif //CATANA_TEXTSINK_HPP
