//
// Created by Michael Bühlmann on 08/02/16.
//

#ifndef CATANA_APP_FILTERS_HPP
#define CATANA_APP_FILTERS_HPP

#include "../types.hpp"

namespace catana{ namespace io {

        //! Base filter class.
        /*
         * Child classes need to implement bool filter(Object&), which returns true if
         * object passes filtering and false if it needs to be removed.
         */
        class Filter {
        public:
            virtual bool filter(Object& object) = 0;

            size_t operator()(ObjectContainer& object_container, bool resize = true);

            virtual size_t operator()(Object* begin, Object* end);
        };

}}

#endif //CATANA_APP_FILTERS_HPP

