//
// Created by Michael Bühlmann on 13/02/16.
//

#ifndef CATANA_WRITE_RAWBINARY_HPP
#define CATANA_WRITE_RAWBINARY_HPP

#include "../record_types.hpp"
#include "../Sink.hpp"
#include <string>
#include <fstream>

namespace catana{ namespace io {

        template<class RecordType>
        class RawBinarySink : public Sink {
        public:
            typedef RecordType record_t;

            RawBinarySink(std::string filename, bool verbose = true);

            ~RawBinarySink();

            virtual long long int write(ObjectContainer::iterator read_iterator, size_t n) override;

            virtual long long int write(Object* read_iterator, size_t n) override;

            void close();

            // Non copyable and assignable
            RawBinarySink(RawBinarySink const&) = delete;

            RawBinarySink& operator=(RawBinarySink const&) = delete;

        private:
            template<class ObjectIterator>
            long long int write_template(ObjectIterator read_iterator, size_t n);

            std::string filename;
            std::ofstream fd;
            bool verbose;
        };

}}
#endif //CATANA_WRITE_RAWBINARY_HPP
