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
     * @param hubble_param The Hubble parameter h which will be used to convert Mpc/h -> Mpc. If coordinates
     * are already in Mpc, set h=1.
     * @param box_size box_size will be used to shift origin to center of the box (i.e. coordinates
     * will be transformed x -> x-box_size/2.). If origin already located at center of box, set box_size=0.
     */
    TextSource(std::string filename,
               double hubble_param = 1, double box_size = 0);

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
    double hubble_param;
    double box_size;
    std::ifstream fs;
  };

}}
#endif //CATANA_TEXTSOURCE_HPP
