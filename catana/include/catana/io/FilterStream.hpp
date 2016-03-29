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

    //! The FilterStream class combines source, filters and sink to a stream.
    /*
     * The pipeline can be started by run. Data is loaded from "source" in chuncks of size "buffer_size"
     * until source.read returns -1. The data is filtered by the filters added by "add_filter" and stored
     * into the "sink".
     * If only a random subset of the filtered data shall be stored into sink, set "subsample_size" > 0. The data
     * will then be first filtered and stored in a temporary file specified by "temp_file_name", from where
     * a random subsample will be taken and stored into "sink".
     * subsample_size == 0 means no subsampling, i.e. all data from the filtered source is moved to "sink".
     */
    class FilterStream {
    public:
        //! Construct from Source and Sink
        /*!
         * @param source pointer to an instance of Source
         * @param sink   pointer to an instance of Sink
         * @param buffer_size data will be read, filtered and stored in chuncks of buffer_size
         * @param subsample_size size of the random subset taken from filtered data which is stored to sink. Set to 0
         * if no subsampling shall be done.
         * @param temp_file_name If subsampling is turned on, name the filename of the temporary file to store data
         * before subsampling
         * @param verbose Set to true for additional output to stdout.
         */
        FilterStream(Source* source, Sink* sink, size_t buffer_size = 1000000,
                size_t subsample_size = 0, std::string temp_file_name = "tmp.bin",
                bool verbose = false);

        //! Add filter to the list of filters which will be applied during .run()
        void add_filter(Filter* filter_p);

        //! Read data from source, filter it and write to sink.
        /*!
         * @return number of objects written to sink.
         */
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
