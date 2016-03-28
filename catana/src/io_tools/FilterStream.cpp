//
// Created by Michael Bühlmann on 15/02/16.
//

//
// Created by Michael Bühlmann on 11/02/16.
//

#include <catana/io_tools/FilterStream.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <random>
#include <set>

extern std::mt19937 rng;

namespace catana { namespace io {

    class RandomSubsetFilter : public Filter {
    public:
        RandomSubsetFilter(size_t n_subset, size_t n_tot)
                :current(0)
        {
            assert(n_subset<n_tot);
            size_t n_added = 0;
            std::uniform_int_distribution<size_t> random_dist(0, n_tot-1);
            while (n_added<n_subset) {
                auto ret = subset_indices.insert(random_dist(rng));
                if (ret.second)
                    ++n_added;
            }
        }

        bool operator()(size_t n)
        {
            return (subset_indices.find(n)!=subset_indices.end());
        }

        void reset()
        {
            current = 0;
        }

        virtual bool filter(Object& object)
        {
            return operator()(current++);
        };

    private:
        std::set<size_t> subset_indices;
        size_t current;
    };

    FilterStream::FilterStream(Source* source, Sink* sink, size_t buffer_size, size_t subset_size,
            std::string temp_file_name, bool verbose)
            :source(source),
             sink(sink),
             buffer(new Object[buffer_size]),
             buffer_size(buffer_size),
             temp_file_name(temp_file_name),
             subsample_size(subset_size),
             verbose(verbose) { }

    void FilterStream::add_filter(Filter* filter_p)
    {
        filters_p.push_back(filter_p);
    }

    size_t FilterStream::run()
    {
        size_t n;
        if (subsample_size==0) {
            n = run_step(source, sink, filters_p);
        }
        else {
            auto temp_sink = new RawBinarySink<SphericalRecord<double>>(temp_file_name, verbose);
            n = run_step(source, temp_sink, filters_p);
            delete temp_sink;

            std::vector<Filter*> fi_p;
            std::unique_ptr<Filter> random_subset_filter; // Declare here such that it can outlive the if {}
            if (subsample_size<n) {
                random_subset_filter.reset(new RandomSubsetFilter(subsample_size, n));
                fi_p = {random_subset_filter.get()};
            }  // Else: no filter in list -> no filters applied

            auto temp_source = new RawBinarySource<SphericalRecord<double>>(temp_file_name, verbose);
            n = run_step(temp_source, sink, fi_p);
            delete temp_source;  // Close temporary file
            std::remove(temp_file_name.c_str());  // Delete temporary file
        }
        return n;
    }

    size_t FilterStream::run_step(Source* so,
            Sink* si, std::vector<Filter*>& fi_p)
    {
        std::cout.fill(' ');
        long long int n;
        size_t n_tot = 0;
        size_t n_read = 0;
        while ((n = so->read(buffer.get(), buffer_size))!=-1) { // Read until nothing left to read
            assert(n<=buffer_size);
            n_read += n;
            if (verbose)
                std::cout << "\tObjects read: " << std::setw(10) << n << " (" << std::setw(10) << n_read << ")\t";
            // Apply filters
            for (auto& filter_p: fi_p) {
                n = (*filter_p)(buffer.get(), buffer.get()+n);
            }
            // Write
            n = si->write(buffer.get(), n);
            n_tot += n;
            if (verbose) {
                std::cout << "written: " << std::setw(10) << n << " ( " << std::setw(10) << n_tot << ")"
                        << std::endl;
            }
        }
        return n_tot;
    }

}}