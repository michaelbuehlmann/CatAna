#ifndef CATANA_TEXTSOURCE_HPP
#define CATANA_TEXTSOURCE_HPP

#include "../Source.hpp"

#include <string>
#include <fstream>


namespace catana { namespace io {

  //! A Source class for text files. The Points are assumed to be stored in the RecordType Format.
  /*!
   * @tparam RecordType A class defined in the record_types.hpp file. Determines the storage order in the binary
   * file (cartesian, spherical, coordinate order)
   */
  template<class RecordType>
  class TextSource : public Source {
    typedef RecordType record_t;
  public:
    //! Construct from filename.
    /*!
     * @param filename filename of the text file
     * @param shift shift coordinates along all axis (coordinates will be transformed x -> x+shift)
     */
    TextSource(std::string filename, double shift = 0);

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

    //! Resets Source to initial state (everything can be read again)
    void reset() override;

  private:
    template<class PointIterator>
    long long int read_template(PointIterator write_iterator, size_t n);

  private:
    std::string filename;
    double shift;
    std::ifstream fs;
  };

}}
#endif //CATANA_TEXTSOURCE_HPP
