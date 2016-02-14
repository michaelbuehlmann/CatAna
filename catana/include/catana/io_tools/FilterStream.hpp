//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_FILTERSTREAMS_HPP
#define CATANA_FILTERSTREAMS_HPP

#include "filters.hpp"
#include "Source.hpp"
#include "Sink.hpp"
#include "sinks/write_rawbinary.hpp"
#include "sources/read_rawbinary.hpp"
#include "../types.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <random>
#include <set>

extern std::mt19937 rng;

class RandomSubsetFilter : public Filter{
public:
    RandomSubsetFilter(size_t n_subset, size_t n_tot)
            :current(0)
    {
        assert(n_subset < n_tot);
        size_t n_added = 0;
        size_t rand;
        std::uniform_int_distribution<size_t> random_dist(0, n_tot-1);
        while(n_added < n_subset){
            auto ret = subset_indices.insert(random_dist(rng));
            if(ret.second)
                ++n_added;
        }
    }
    bool operator()(size_t n){
        return (subset_indices.find(n) != subset_indices.end());
    }
    void reset(){
        current = 0;
    }

    virtual bool filter(Object& object) {
        return operator()(current++);
    };

private:
    std::set<size_t> subset_indices;
    size_t current;
};


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
              subset_size(0),
              buffer(new Object[buffer_size]),
              verbose(verbose)
    { }

    FilterStream(Source* source, Sink* sink, size_t buffer_size, size_t subset_size, std::string temp_file_name, bool verbose=true)
            : source(source),
              sink(sink),
              buffer_size(buffer_size),
              subset_size(subset_size),
              temp_file_name(temp_file_name),
              buffer(new Object[buffer_size]),
              verbose(verbose)
    { }

    // Method to add filter to the list of filters
    void add_filter(Filter* filter_p) {filters_p.push_back(filter_p);}

    // Read all data, filter and write.
    size_t run() {
        size_t n;
        if(subset_size==0){
            n = run_step(source, sink, filters_p);
        } else {
            auto temp_sink = new RawBinarySink<SphericalRecord<double>>(temp_file_name, verbose);
            n = run_step(source, temp_sink, filters_p);
            delete temp_sink;

            std::vector<Filter*> fi_p;
            std::unique_ptr<Filter> random_subset_filter; // Declare here such that it can outlive the if {}
            if(subset_size<n){
                random_subset_filter.reset(new RandomSubsetFilter(subset_size, n));
                fi_p = {random_subset_filter.get()};
            }  // Else: no filter in list -> no filters applied

            auto temp_source = new RawBinarySource<SphericalRecord<double>>(temp_file_name, verbose);
            n = run_step(temp_source, sink, fi_p);
            delete temp_source;  // Close temporary file
            std::remove(temp_file_name.c_str());  // Delete temporary file
        }
        return n;
    }

private:
    size_t run_step(Source* so,
            Sink* si, std::vector<Filter*>& fi_p)
    {
        std::cout.fill(' ');
        long long int n;
        size_t n_tot = 0;
        size_t n_read = 0;
        while((n = so->read(buffer.get(), buffer_size)) != -1){ // Read until nothing left to read
            assert(n<=buffer_size);
            n_read+=n;
            if(verbose)
                std::cout << "\tObjects read: " << std::setw(10) << n << " (" <<std::setw(10) << n_read << ")\t";
            // Apply filters
                for(auto& filter_p: fi_p) {
                    n = (*filter_p)(buffer.get(), buffer.get()+n);
                }
            // Write
            n = si->write(buffer.get(), n);
            n_tot += n;
            if(verbose) {
                std::cout << "written: " << std::setw(10) << n << " ( "<< std::setw(10) << n_tot << ")" << std::endl;
            }
        }
        return n_tot;
    }

private:
    // TODO: use smart pointers for memory management.
    Source* source;
    Sink* sink;
    std::vector<Filter*> filters_p;
    std::unique_ptr<Object[]> buffer;
    size_t buffer_size;
    std::string temp_file_name;
    size_t subset_size;

    bool verbose;
};

// TODO: Close function for FilterStream (closes HDF5 file)

#endif //CATANA_FILTERSTREAMS_HPP
