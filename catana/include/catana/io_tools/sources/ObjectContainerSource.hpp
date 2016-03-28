//
// Created by Michael Bühlmann on 24/02/16.
//

#ifndef CATANA_OBJECTCONTAINERSOURCE_HPP
#define CATANA_OBJECTCONTAINERSOURCE_HPP

#include "../Source.hpp"

namespace catana{ namespace io {

        class ObjectContainerSource : public Source {
        public:
            ObjectContainerSource(const ObjectContainer& object_container);

            long long int read(ObjectContainer::iterator write_iterator, size_t n) override;

            long long int read(Object* write_iterator, size_t n) override;

            void reset() override;

            size_t get_nobjects() override;

        private:
            template<class ObjectIterator>
            long long int read_template(ObjectIterator write_iterator, size_t n);

        private:
            const ObjectContainer& object_container;
            ObjectContainer::const_iterator current;
        };

}}
#endif //CATANA_OBJECTCONTAINERSOURCE_HPP
