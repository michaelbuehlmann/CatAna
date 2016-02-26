//
// Created by Michael Bühlmann on 26/02/16.
//

#ifndef CATANA_TEXTSINK_HPP
#define CATANA_TEXTSINK_HPP

#include "../record_types.hpp"
#include "../Sink.hpp"
#include <string>
#include <fstream>

namespace catana{ namespace io {

        template<class RecordType>
        class TextSink : public Sink {
        public:
            typedef RecordType record_t;

            TextSink(std::string filename, bool verbose = true);

            virtual long long int write(ObjectContainer::iterator read_iterator, size_t n) override;

            virtual long long int write(Object* read_iterator, size_t n) override;

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
