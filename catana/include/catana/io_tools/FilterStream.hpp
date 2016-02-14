//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_FILTERSTREAMS_HPP
#define CATANA_FILTERSTREAMS_HPP

#include "filters.hpp"
#include "Source.hpp"
#include "Sink.hpp"
#include "../types.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>


//! The FilterStream class combines input, filters and output.
/*
 * Filters can be added by add
 * The pipeline can be started by run
 */
class FilterStream {
public:
    FilterStream(Source* source, Sink* sink, size_t buffer_size = 1000000, bool verbose=true)
            : source(source),
              sink(sink),
              buffer_size(buffer_size),
              buffer(new Object[buffer_size]),
              verbose(verbose)
    { }

    // Method to add filter to the list of filters
    void add_filter(Filter* filter_p) {filters_p.push_back(filter_p);}

    // Read all data, filter and write.
    size_t run() {
        long long int n;
        size_t n_tot = 0;
        size_t n_read = 0;
        while((n = source->read(buffer.get(), buffer_size)) != -1){ // Read until nothing left to read
            if(verbose) {
                n_read += n;
                std::cout << "\tObjects read: " << n_read << std::endl;
            }
            assert(n<=buffer_size);
            for(auto& filter_p: filters_p) {
                n = (*filter_p)(buffer.get(), buffer.get()+n);
            }
            n_tot += sink->write(buffer.get(), n);
        }
        if(verbose)
            std::cout << "Done. Total objects written to new file: " << n_tot << std::endl;
        return n_tot;
    }
private:
    // TODO: use smart pointers for memory management.
    Source* source;
    Sink* sink;
    std::vector<Filter*> filters_p;
    std::unique_ptr<Object[]> buffer;
    size_t buffer_size;

    bool verbose;
};

// TODO: Close function for FilterStream (closes HDF5 file)

#endif //CATANA_FILTERSTREAMS_HPP
