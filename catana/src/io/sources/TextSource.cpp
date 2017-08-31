#include <catana/io/sources/TextSource.hpp>
#include <catana/io/record_types.hpp>


namespace catana { namespace io {

  template<class RecordType>
  TextSource<RecordType>::TextSource(std::string filename, double shift)
      :filename(filename), shift(shift) {
    fs.open(filename, std::fstream::in);
    if(!fs.is_open()) {
      std::cout << "WARNING: Could not open file " << filename << std::endl;
    }
  }

  template<class RecordType>
  template<class PointIterator>
  long long int TextSource<RecordType>::read_template(PointIterator write_iterator, size_t n) {
    size_t i = 0;
    typedef typename record_t::float_t float_t;
    float_t arg1, arg2, arg3;
    std::string line;
    while(i < n) {
      if(std::getline(fs, line)) {
        std::istringstream in(line);
        in >> arg1;
        in >> arg2;
        in >> arg3;
        *(write_iterator++) = record_t(arg1, arg2, arg3).point(shift);
        ++i;
      } else {
        break;
      }
    }

    if(i == 0) {
      return -1;
    } else {
      return i;
    }
  }

  template<class RecordType>
  long long int TextSource<RecordType>::read(Point *write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  template<class RecordType>
  long long int TextSource<RecordType>::read(PointContainer::iterator write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  template<class RecordType>
  void TextSource<RecordType>::reset() {
    fs.clear();
    fs.seekg(0, std::ios::beg);
  }


  ////////////////////////////
  // Implicit instantiation //
  template
  class TextSource<CartesianRecord<float>>;


  template
  class TextSource<CartesianRecord<double>>;


  template
  class TextSource<SphericalRecord<float, SphericalTextFormat::RTHETAPHI>>;


  template
  class TextSource<SphericalRecord<double, SphericalTextFormat::RTHETAPHI>>;


  template
  class TextSource<SphericalRecord<float, SphericalTextFormat::THREEDEX>>;


  template
  class TextSource<SphericalRecord<double, SphericalTextFormat::THREEDEX>>;

}}