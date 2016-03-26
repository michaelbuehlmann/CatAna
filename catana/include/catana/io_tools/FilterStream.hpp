//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_FILTERSTREAMS_HPP
#define CATANA_FILTERSTREAMS_HPP

#include "Filter.hpp"
#include "Source.hpp"
#include "Sink.hpp"
#include "sinks/RawBinarySink.hpp"
#include "sources/RawBinarySource.hpp"
#include "../types.hpp"

#include <memory>
#include <vector>

namespace catana { namespace io {
    //! The FilterStream class combines input, filters and output.
    /*
     * Filters can be added by add
     * The pipeline can be started by run
     */

    class FilterStream {
    public:
//        FilterStream(Source* source, Sink* sink, size_t buffer_size = 1000000, bool verbose = false);

        FilterStream(Source* source, Sink* sink, size_t buffer_size = 1000000,
                size_t subsample_size = 0, std::string temp_file_name = "tmp.bin",
                bool verbose = false);

        // Method to add filter to the list of filters
        void add_filter(Filter* filter_p);

        // Read all data, filter and write.
        size_t run();

    private:
        size_t run_step(Source* so, Sink* si, std::vector<Filter*>& fi_p);

    private:
        // TODO: use smart pointers for memory management.
        Source* source;
        Sink* sink;
        std::vector<Filter*> filters_p;
        std::unique_ptr<Object[]> buffer;
        size_t buffer_size;
        std::string temp_file_name;
        size_t subsample_size;

        bool verbose;
    };

}}
#endif //CATANA_FILTERSTREAMS_HPP
