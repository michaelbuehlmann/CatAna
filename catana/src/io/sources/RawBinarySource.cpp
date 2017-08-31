#include <catana/io/sources/RawBinarySource.hpp>


namespace catana { namespace io {

  template<class RecordType>
  RawBinarySource<RecordType>::RawBinarySource(std::string filename, bool verbose)
      :filename(filename), current_pos(0), verbose(verbose) {
    // Open file at the end
    fd.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if(!fd.is_open()) {
      std::cout << "WARNING: Could not open file " << filename << std::endl;
    } else {
      if(verbose)
        std::cout << "Opened file " << filename << std::endl;

      // Get number of points (size of file / size of record)
      ntot = static_cast<size_t>(fd.tellg() / sizeof(record_t));

      // Move to front
      fd.seekg(0, std::ios::beg);
    }
  }


//template<class RecordType>
//RawBinarySource<RecordType>::~RawBinarySource()
//{
//    close();
//}
//template<class RecordType>
//void RawBinarySource<RecordType>::close()
//{
//    fd.close();
//    if(verbose)
//        std::cout << "Closed file " << filename << std::endl;
//}

  template<class RecordType>
  long long int RawBinarySource<RecordType>::read(PointContainer::iterator write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  template<class RecordType>
  long long int RawBinarySource<RecordType>::read(Point *write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  template<class RecordType>
  template<class PointIterator>
  long long int RawBinarySource<RecordType>::read_template(PointIterator write_iterator, size_t n) {
    if(!fd.is_open())
      return -1;
    record_t record;
    size_t to_read = std::min(n, static_cast<size_t>(ntot - current_pos));
    if(to_read == 0) {
      return -1;
    } else {
      for(size_t i = 0; i < to_read; ++i) {
        fd.read((char *) &record, sizeof(record));
        *(write_iterator++) = record.point(0);
        ++current_pos;
      }
      return to_read;
    }
  }

  template<class RecordType>
  void RawBinarySource<RecordType>::reset() {
    current_pos = 0;
    fd.seekg(0, std::ios::beg);
  }

  template<class RecordType>
  size_t RawBinarySource<RecordType>::get_npoints() {
    return ntot;
  }


//////////////////////////
// Implicit instantiation
  template
  class RawBinarySource<CartesianRecord<float>>;


  template
  class RawBinarySource<CartesianRecord<double>>;


  template
  class RawBinarySource<SphericalRecord<float>>;


  template
  class RawBinarySource<SphericalRecord<double>>;

}}