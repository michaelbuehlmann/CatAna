#ifndef CATANA_FILTERSTREAMS_HPP
#define CATANA_FILTERSTREAMS_HPP

#include "Filter.hpp"
#include "Source.hpp"
#include "Sink.hpp"
#include "sinks/RawBinarySink.hpp"
#include "sources/RawBinarySource.hpp"

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
    FilterStream(Source *source, Sink *sink, size_t buffer_size = 1000000, bool verbose = false);

    //! Set the data source
    void set_source(Source *source);

    //! Add filter to the list of filters which will be applied during .run()
    void add_filter(Filter *filter_p);

    //! Read data from source, filter it and write to sink.
    /*!
     * @return number of points written to sink.
     */
    size_t run(size_t subsample_size = 0, std::string temp_filename = "tmp.bin");

    //! Run intermediate steps manually: write source to temporary file, apply filters
    /*!
     * This function is meant to use if multiple sources need to be combined and then subsampled (e.g. if data is
     * spread across multiple files). Run the "run_totemp" function with the same temporary file for all sources first and then
     * run the "run_fromtemp" function with the number of particles needed.
     *
     * @param temp_filename Path and name of the temporary file
     * @return number of points written to temporary file
     */
    size_t run_totemp(std::string temp_filename = "tmp.bin", bool append = true);

    //! Run intermediate steps manually: write temporary file to sink (with subsampling)
    /*!
     * @param subsample_size The number of particles which are randomly drawn from the temporary file and moved to
     * the sink point
     * @param temp_filename Path and name of the temporary file
     * @return number of points written to sink
     */
    size_t run_fromtemp(std::string temp_filename = "tmp.bin", size_t subsample_size = 0, bool remove_temp = true);


  private:
    size_t run_step(Source *so, Sink *si, std::vector<Filter *>& fi_p);

  private:
    // TODO: use smart pointers for memory management.
    Source *source;
    Sink *sink;
    std::vector<Filter *> filters_p;
    std::unique_ptr<Point[]> buffer;
    size_t buffer_size;

    bool verbose;
  };

}}
#endif //CATANA_FILTERSTREAMS_HPP
