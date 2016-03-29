//
// Created by Michael Bühlmann on 08/02/16.
//

#ifndef CATANA_APP_FILTERS_HPP
#define CATANA_APP_FILTERS_HPP

#include "../types.hpp"

namespace catana { namespace io {

    //! Parent class Filter for filtering data of type Object class.
    /*!
     * Child classes need to implement bool filter(Object&), which returns true if
     * object passes filtering and false if it needs to be removed.
     */
    class Filter {
    public:
        //! Filtering function on Object. Returns true if object passes filter, false otherwise.
        virtual bool filter(Object& object) = 0;

        //! Run filter on an object_container
        /*!
         * @param object_container object_container on which to apply filter
         * @param resize if true, resize the object container. if false, the filtered data will be within
         * [object_container.begin(), object_container.begin()+n), where n is the return value
         * @return number of objects remaining after filtering (== object_container.size() if resize==true)
         */
        size_t operator()(ObjectContainer& object_container, bool resize = true);

        //! Run filter on a C-array of Objects
        /*!
         * Filter objects between [begin, end). Remaining objects will be stored between [begin, begin +n), where
         * n is the return value
         * @return number of objects remaining after filtering.
         */
        virtual size_t operator()(Object* begin, Object* end);
    };

}}

#endif //CATANA_APP_FILTERS_HPP

