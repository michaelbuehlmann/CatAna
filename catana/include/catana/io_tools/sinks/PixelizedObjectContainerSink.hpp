//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_PIXELIZEDOBJECTCONTAINERSINK_HPP
#define CATANA_PIXELIZEDOBJECTCONTAINERSINK_HPP

#include "../Sink.hpp"

namespace catana{ namespace io {

        class PixelizedObjectContainerSink : public Sink {
        public:
            PixelizedObjectContainerSink(PixelizedObjectContainer& pixelized_object_container)
                    :pixelized_object_container(pixelized_object_container) { };

            long long int write(ObjectContainer::iterator read_iterator, size_t n)
            {
                return write_template(read_iterator, n);
            };

            long long int write(Object* read_iterator, size_t n) { return write_template(read_iterator, n); };
        private:
            template<class ObjectIterator>
            long long int write_template(ObjectIterator read_iterator, size_t n)
            {
                for (size_t i = 0; i<n; ++i) {
                    pixelized_object_container.add_object(*(read_iterator++));
                }
                return n;
            }

        private:
            PixelizedObjectContainer& pixelized_object_container;
        };

}}
#endif //CATANA_PIXELIZEDOBJECTCONTAINERSINK_HPP
