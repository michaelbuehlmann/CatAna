#ifndef CATANA_READ_RAWBINARY_HPP_HPP
#define CATANA_READ_RAWBINARY_HPP_HPP

#include "../record_types.hpp"
#include "../Source.hpp"
#include <string>
#include <fstream>


namespace catana { namespace io {

  //! A Source class for raw binary files. The Points are assumed to be stored in the RecordType Format.
  /*!
   * @tparam RecordType A class defined in the record_types.hpp file. Determines the storage order in the binary
   * file.
   */
  template<class RecordType>
  class RawBinarySource : public Source {
  public:
    typedef RecordType record_t;

    //! Construct from filename.
    RawBinarySource(std::string filename, bool verbose = true);

    //! Read next n points from file. Returns number of points put into point_s. -1 if EOF.
    /*!
     * @param write_iterator iterator of PointContainer of points. [write_iterator, write_iterator + n]
     * must be a valid range.
     * @param n number of points to read
     */
    virtual long long int read(PointContainer::iterator write_iterator, size_t n) override;

    //! Read next n points from. Returns number of points put into point_s. -1 if EOF.
    /*!
     * @param write_iterator pointer to a Point[] C-array of points. [write_iterator, write_iterator + n]
     * must be a valid range.
     * @param n number of points to read
     */
    virtual long long int read(Point *write_iterator, size_t n) override;

    //! Total number of points which can be read (i.e. size of point_container)
    size_t get_npoints() override;

    //! Resets Source to initial state (everything can be read again)
    void reset() override;

    // Non copyable and assignable
    RawBinarySource(RawBinarySource const&) = delete;

    RawBinarySource& operator=(RawBinarySource const&) = delete;

  private:
    template<class PointIterator>
    long long int read_template(PointIterator write_iterator, size_t n);

    std::string filename;
    std::ifstream fd;
    size_t ntot;
    size_t current_pos;
    bool verbose;
  };

}}
#endif //CATANA_READ_RAWBINARY_HPP_HPP
