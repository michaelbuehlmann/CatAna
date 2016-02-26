//
// Created by Michael Bühlmann on 13/02/16.
//

#include <catana/io_tools/sinks/RawBinarySink.hpp>

namespace catana{ namespace io {

        template<class RecordType>
        long long int RawBinarySink<RecordType>::write(ObjectContainer::iterator read_iterator, size_t n)
        {
            return write_template(read_iterator, n);
        }

        template<class RecordType>
        long long int RawBinarySink<RecordType>::write(Object* read_iterator, size_t n)
        {
            return write_template(read_iterator, n);
        }

        template<class RecordType>
        RawBinarySink<RecordType>::RawBinarySink(std::string filename, bool verbose)
                :filename(filename), verbose(verbose)
        {
            fd.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
            if (!fd.is_open()) {
                std::cout << "WARNING: Could not create file " << filename << std::endl;
            }
            else {
                if (verbose)
                    std::cout << "Created file " << filename << std::endl;
            }
        }

        template<class RecordType>
        RawBinarySink<RecordType>::~RawBinarySink()
        {
            close();
        }

        template<class RecordType>
        void RawBinarySink<RecordType>::close()
        {
            fd.close();
            if (verbose)
                std::cout << "Closed file " << filename << std::endl;
        }

        template<class RecordType>
        template<class ObjectIterator>
        long long int RawBinarySink<RecordType>::write_template(ObjectIterator read_iterator, size_t n)
        {
            if (!fd.is_open())
                return -1;

            record_t record;
            for (size_t i = 0; i<n; ++i) {
                record = record_t(*read_iterator++, 0, 1);
                fd.write((char*) &record, sizeof(record_t));
            }
            fd.flush();
            return static_cast<long long int>(n);
        }

        //////////////////////////
        // Implicit instantiation
        template
        class RawBinarySink<CartesianRecord<float>>;

        template
        class RawBinarySink<CartesianRecord<double>>;

        template
        class RawBinarySink<SphericalRecord<float>>;

        template
        class RawBinarySink<SphericalRecord<double>>;

}}