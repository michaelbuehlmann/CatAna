//
// Created by Michael Bühlmann on 14/02/16.
//

#ifndef CATANA_READ_RAWBINARY_HPP_HPP
#define CATANA_READ_RAWBINARY_HPP_HPP

#include "../record_types.hpp"
#include "../Source.hpp"
#include <string>
#include <fstream>

namespace catana{ namespace io {

        template<class RecordType>
        class RawBinarySource : public Source {
        public:
            typedef RecordType record_t;

            RawBinarySource(std::string filename, bool verbose = true);

            virtual long long int read(ObjectContainer::iterator write_iterator, size_t n) override;
            virtual long long int read(Object* write_iterator, size_t n) override;

            size_t get_nobjects() override;
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
