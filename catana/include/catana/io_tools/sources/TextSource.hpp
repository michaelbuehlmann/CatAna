//
// Created by Michael Bühlmann on 23/02/16.
//

#ifndef CATANA_TEXTSOURCE_HPP
#define CATANA_TEXTSOURCE_HPP

#include "../Source.hpp"

#include <string>
#include <fstream>

namespace catana{ namespace io {
    template<class RecordType>
    class TextSource : public Source {
        typedef RecordType record_t;
    public:
        TextSource(std::string filename,
                double hubble_param = 1, double box_size = 0);
        virtual long long int read(ObjectContainer::iterator write_iterator, size_t n) override;
        virtual long long int read(Object* write_iterator, size_t n) override;
        void reset() override;

    private:
        template<class ObjectIterator>
        long long int read_template(ObjectIterator write_iterator, size_t n);

    private:
        std::string filename;
        double hubble_param;
        double box_size;
        std::ifstream fs;
    };

}}
#endif //CATANA_TEXTSOURCE_HPP
