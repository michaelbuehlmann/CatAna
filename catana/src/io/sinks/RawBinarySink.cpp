#include <catana/io/sinks/RawBinarySink.hpp>


namespace catana { namespace io {

  template<class RecordType>
  long long int RawBinarySink<RecordType>::write(PointContainer::const_iterator read_iterator, size_t n) {
    return write_template(read_iterator, n);
  }

  template<class RecordType>
  long long int RawBinarySink<RecordType>::write(Point *read_iterator, size_t n) {
    return write_template(read_iterator, n);
  }

  template<class RecordType>
  RawBinarySink<RecordType>::RawBinarySink(std::string filename, bool verbose, bool append)
      :filename(filename), verbose(verbose) {
    auto mode = std::ios::out | std::ios::binary;
    if(append) {
      if(verbose)
        std::cout << "Opened RawBinarySink " << filename << " in append-mode" << std::endl;
      mode = mode | std::ios::app | std::ios::ate;
    } else {
      if(verbose) {
        std::cout << "Opened RawBianrySink " << filename << " in overwrite-mode" << std::endl;
      }
      mode = mode | std::ios::trunc;
    }
    fd.open(filename, mode);
    if(!fd.is_open()) {
      std::cout << "WARNING: Could not create file " << filename << std::endl;
    }
  }

  template<class RecordType>
  RawBinarySink<RecordType>::~RawBinarySink() {
    close();
  }

  template<class RecordType>
  void RawBinarySink<RecordType>::close() {
    fd.close();
    if(verbose)
      std::cout << "Closed file " << filename << std::endl;
  }

  template<class RecordType>
  template<class PointIterator>
  long long int RawBinarySink<RecordType>::write_template(PointIterator read_iterator, size_t n) {
    if(!fd.is_open())
      return -1;

    record_t record;
    for(size_t i = 0; i < n; ++i) {
      record = record_t(*read_iterator++, 0);
      fd.write((char *) &record, sizeof(record_t));
    }
    fd.flush();
    return static_cast<long long int>(n);
  }


  //////////////////////////
  // Implicit instantiation
  template
  class RawBinarySink<CartesianRecord<float>>;


  template
  class RawBinarySink<CartesianRecord<double>>;


  template
  class RawBinarySink<SphericalRecord<float>>;


  template
  class RawBinarySink<SphericalRecord<double>>;

}}