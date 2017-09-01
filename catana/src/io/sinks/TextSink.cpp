#include <catana/io/sinks/TextSink.hpp>
#include <iomanip>
#include <limits>



namespace catana { namespace io {

  template<class RecordType>
  TextSink<RecordType>::TextSink(std::string filename, bool verbose)
      :filename(filename), verbose(verbose) {
    fd.open(filename, std::ios::out | std::ios::trunc);
    if(!fd.is_open()) {
      std::cout << "WARNING: Could not create file " << filename << std::endl;
    } else {
      if(verbose)
        std::cout << "Created file " << filename << std::endl;
      fd.setf(std::ios::scientific, std::ios::floatfield);
      fd.precision(std::numeric_limits<typename record_t::float_t>::max_digits10);
    }
  }

  template<class RecordType>
  long long int TextSink<RecordType>::write(PointContainer::const_iterator read_iterator, size_t n) {
    return write_template(read_iterator, n);
  }

  template<class RecordType>
  long long int TextSink<RecordType>::write(Point *read_iterator, size_t n) {
    return write_template(read_iterator, n);
  }

  template<class RecordType>
  void TextSink<RecordType>::close() {
    fd.close();
  }

  template<class RecordType>
  template<class PointIterator>
  long long int TextSink<RecordType>::write_template(PointIterator read_iterator, size_t n) {
    if(!fd.is_open())
      return -1;

    record_t record;
    for(size_t i = 0; i < n; ++i) {
      record = record_t(*read_iterator++, 0);
      fd << record << "\n";
    }
    fd.flush();
    return static_cast<long long int>(n);
  }


  ////////////////////////////
  // Implicit instantiation //
  template
  class TextSink<CartesianRecord<float>>;


  template
  class TextSink<CartesianRecord<double>>;


  template
  class TextSink<SphericalRecord<float, SphericalTextFormat::RTHETAPHI>>;


  template
  class TextSink<SphericalRecord<double, SphericalTextFormat::RTHETAPHI>>;


  template
  class TextSink<SphericalRecord<float, SphericalTextFormat::THREEDEX>>;


  template
  class TextSink<SphericalRecord<double, SphericalTextFormat::THREEDEX>>;

}}