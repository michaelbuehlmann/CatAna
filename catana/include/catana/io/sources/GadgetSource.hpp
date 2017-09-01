#ifndef CATANA_APP_READ_GADGET_HPP
#define CATANA_APP_READ_GADGET_HPP

#include "../Source.hpp"
#include <string>

#include <iosfwd>
#include <fstream>


namespace catana { namespace io {

  //! A Source class for Binary Gadget files.
  /*!
   * While reading, the coordinates will be automatically transformed from Mpc/h -> Mpc and the origin is moved to
   * the center of the box.
   */
  class GadgetSource : public Source {
  public:
    //! Construct Source from filename
    /*!
     * @param filename Data spread across multiple files will be automatically detected and loaded.  Specify any of the files.
     * @param verbose Set to true for additional output to stdout
     */
    GadgetSource(std::string filename, bool verbose = false);

    //! Read next n points from file(s). Returns number of points put into point_s. -1 if EOF.
    /*!
     * @param write_iterator iterator of PointContainer of Points. [write_iterator, write_iterator + n]
     * must be a valid range.
     * @param n number of points to read
     */
    long long int read(PointContainer::iterator write_iterator, size_t n) override;

    //! Read next n points from file(s). Returns number of points put into point_s. -1 if EOF.
    /*!
     * @param write_iterator pointer to a Point[] C-array of Points. [write_iterator, write_iterator + n]
     * must be a valid range.
     * @param n number of points to read
     */
    long long int read(Point *write_iterator, size_t n) override;

    //! Total number of points which can be read.
    size_t get_npoints() override;

    //! Resets Source to initial state (everything can be read again)
    void reset() override;

    //! The size of the empty elements between blocks in the Gadget file (usually 4 bytes)
    static const int skipsize;
  private:
    template<class PointIterator>
    long long int read_template(PointIterator write_iterator, size_t n);

    void load_file(std::string fname, bool initial_file = false);

    void load_file(unsigned int filenum);

  private:
    std::string filename;
    unsigned int files;
    unsigned int current_file;
    size_t n_points;
    size_t n_points_file;
    size_t current_point;
    size_t current_point_file;

    double box_size;
    double hubble_parameter;

    std::ifstream fd;

    bool verbose;
  };

}}
#endif //CATANA_APP_READ_GADGET_HPP
