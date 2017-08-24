#include <catana/io/FilterStream.hpp>

#include <cassert>
#include <iomanip>
#include <random>
#include <set>


namespace catana {
  extern std::mt19937 rng;

  namespace io {

    class RandomSubsetFilter : public Filter {
    public:
      RandomSubsetFilter(size_t n_subset, size_t n_tot)
          : current(0) {
        assert(n_subset < n_tot);
        size_t n_added = 0;
        std::uniform_int_distribution<size_t> random_dist(0, n_tot - 1);
        while(n_added < n_subset) {
          auto ret = subset_indices.insert(random_dist(rng));
          if(ret.second)
            ++n_added;
        }
      }

      bool operator()(size_t n) {
        return (subset_indices.find(n) != subset_indices.end());
      }

      void reset() {
        current = 0;
      }

      virtual bool filter(Point& point) {
        return operator()(current++);
      };

    private:
      std::set<size_t> subset_indices;
      size_t current;
    };


    FilterStream::FilterStream(Source *source, Sink *sink, size_t buffer_size, bool verbose)
        : source(source),
          sink(sink),
          buffer(new Point[buffer_size]),
          buffer_size(buffer_size),
          verbose(verbose) {}

    void FilterStream::set_source(Source *source) {
      this->source = source;
    }

    void FilterStream::add_filter(Filter *filter_p) {
      filters_p.push_back(filter_p);
    }

    size_t FilterStream::run_totemp(std::string temp_filename, bool append) {
      auto temp_sink = new RawBinarySink<SphericalRecord<double>>(temp_filename, verbose, append);
      size_t n = run_step(source, temp_sink, filters_p);
      delete temp_sink;
      return n;
    }

    size_t FilterStream::run_fromtemp(std::string temp_filename, size_t subsample_size, bool remove_temp) {
      auto temp_source = new RawBinarySource<SphericalRecord<double>>(temp_filename, verbose);

      size_t n = temp_source->get_npoints();
      if(verbose)
        std::cout << "Temporary file " << temp_filename << " contains " << n << " points." << std::endl;
      std::vector<Filter *> fi_p;
      std::unique_ptr<Filter> random_subset_filter;
      if(subsample_size != 0 && subsample_size < n) {
        random_subset_filter.reset(new RandomSubsetFilter(subsample_size, n));
        fi_p = {random_subset_filter.get()};
      }
      n = run_step(temp_source, sink, fi_p);

      delete temp_source;
      if(remove_temp) {
        std::remove(temp_filename.c_str());
      }
      return n;
    }

    size_t FilterStream::run(size_t subsample_size, std::string temp_filename) {
      size_t n;
      if(subsample_size == 0) {
        n = run_step(source, sink, filters_p);
      } else {
        run_totemp(temp_filename, false);
        n = run_fromtemp(temp_filename, subsample_size, true);
      }
      return n;
    }

    size_t FilterStream::run_step(Source *so,
                                  Sink *si, std::vector<Filter *>& fi_p) {
      std::cout.fill(' ');
      long long int n;
      size_t n_tot = 0;
      size_t n_read = 0;
      while((n = so->read(buffer.get(), buffer_size)) != -1) { // Read until nothing left to read
        assert(n <= buffer_size);
        n_read += n;
        if(verbose)
          std::cout << "\tPoints read: " << std::setw(10) << n << " (" << std::setw(10) << n_read << ")\t";
        // Apply filters
        for(auto& filter_p: fi_p) {
          n = (*filter_p)(buffer.get(), buffer.get() + n);
        }
        // Write
        n = si->write(buffer.get(), n);
        n_tot += n;
        if(verbose) {
          std::cout << "written: " << std::setw(10) << n << " ( " << std::setw(10) << n_tot << ")"
                    << std::endl;
        }
      }
      return n_tot;
    }

  }
}