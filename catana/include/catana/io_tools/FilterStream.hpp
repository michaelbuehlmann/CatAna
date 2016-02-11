//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_FILTERSTREAMS_HPP
#define CATANA_FILTERSTREAMS_HPP

#include "filters.hpp"
#include "Source.hpp"
#include "Sink.hpp"
#include <catana/types.hpp>

#include <vector>
#include <memory>
#include <algorithm>

class FilterStream {
public:
    FilterStream(Source* source, Sink* sink, size_t buffer_size = 1000000)
            : source(source),
              sink(sink),
              buffer_size(buffer_size),
              buffer(new Object[buffer_size])
    { }

    // Method to add filter to the list of filters
    void add_filter(Filter* filter_p) {filters_p.push_back(filter_p);}

    // Read all data, filter and write.
    size_t run() {
        long long int n;
        size_t n_tot = 0;
        while((n = source->read(buffer.get(), buffer_size)) != -1){ // Read until nothing left to read
            assert(n<=buffer_size);
            for(auto& filter_p: filters_p) {
                n = (*filter_p)(buffer.get(), buffer.get()+n);
            }
            n_tot += sink->write(buffer.get(), n);
        }
        return n_tot;
    }
private:
    Source* source;
    Sink* sink;
    std::vector<Filter*> filters_p;
    std::unique_ptr<Object[]> buffer;
    size_t buffer_size;
};

#endif //CATANA_FILTERSTREAMS_HPP
