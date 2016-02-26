//
// Created by Michael Bühlmann on 24/02/16.
//

#include <catana/io_tools/sources/ObjectContainerSource.hpp>

namespace catana{ namespace io {

        ObjectContainerSource::ObjectContainerSource(const ObjectContainer& object_container)
                :object_container(object_container), current(object_container.begin()) { };

        long long int ObjectContainerSource::read(ObjectContainer::iterator write_iterator, size_t n)
        {
            return read_template(write_iterator, n);
        }

        long long int ObjectContainerSource::read(Object* write_iterator, size_t n)
        {
            return read_template(write_iterator, n);
        }

        void ObjectContainerSource::reset()
        {
            current = object_container.begin();
        }

        size_t ObjectContainerSource::get_nobjects()
        {
            return object_container.size();
        }

        template<class ObjectIterator>
        long long int ObjectContainerSource::read_template(ObjectIterator write_iterator, size_t n)
        {
            size_t to_read = std::min(n, static_cast<size_t>(std::distance(current, object_container.end())));

            // Abort if nothing to read
            if (to_read==0) {
                return -1;
            }

            for (size_t i = 0; i<to_read; ++i) {
                *(write_iterator++) = *(current++);
            }
            return to_read;
        }

}}