#include "catana/io/sources/GadgetSource.hpp"
#include "catana/io/record_types.hpp"
#include "catana/tools/string_format.hpp"

#include <cassert>


namespace catana { namespace io {

  //! HEADER block defined in gadget manual/read_snapshot.c
  struct io_header {
    unsigned int npart[6];
    double mass[6];
    double time;
    double redshift;
    int flag_sfr;
    int flag_feedback;
    int npartTotal[6];
    int flag_cooling;
    int num_files;
    double BoxSize;
    double Omega0;
    double OmegaLambda;
    double HubbleParam;
    char fill[256 - 6 * 4 - 6 * 8 - 2 * 8 - 2 * 4 - 6 * 4 - 2 * 4 - 4 * 8];    /* fills to 256 Bytes */
  };

  std::ostream& operator<<(std::ostream& stream, const io_header header) {
    stream << "Header:" << std::endl;
    stream << "\tNumFiles: " << header.num_files << std::endl;
    stream << "\t" << string_format("N_Gas: %d (%d)", header.npart[0], header.npartTotal[0]) << std::endl;
    stream << "\t" << string_format("N_Halo: %d (%d)", header.npart[1], header.npartTotal[1]) << std::endl;
    stream << "\t" << string_format("N_Disk: %d (%d)", header.npart[2], header.npartTotal[2]) << std::endl;
    stream << "\t" << string_format("N_Bulge: %d (%d)", header.npart[3], header.npartTotal[3]) << std::endl;
    stream << "\t" << string_format("N_Stars: %d (%d)", header.npart[4], header.npartTotal[4]) << std::endl;
    stream << "\t" << string_format("N_Bndry: %d (%d)", header.npart[5], header.npartTotal[5]) << std::endl;
    stream << "\tRedshift: " << header.redshift << std::endl;
    stream << "\tBoxSize: " << header.BoxSize << std::endl;
    stream << "\tHubbleParam: " << header.HubbleParam << std::endl;

    return stream;
  }

  const int GadgetSource::skipsize = 4;

  GadgetSource::GadgetSource(std::string filename, bool verbose)
      : filename(filename), current_point(0), current_point_file(0), verbose(verbose) {
    load_file(filename, true);
    if(files > 1) {
      // Remove last digit of filename
      this->filename.erase(filename.find_last_of('.'), std::string::npos);

      if(verbose) {
        std::cout << "Detected " << files << " files." << std::endl;
        std::cout << "Current file: " << this->filename << " (" << current_file << ")" << std::endl;
      }

      // In case filename does not end with '.0' (first file), load first file
      if(current_file != 0) {
        if(verbose)
          std::cout << "Provided file " << current_file << " instead of 0. Reloading first file."
                    << std::endl;

        load_file(0);
      }
    }
  }

  template<class PointIterator>
  long long int GadgetSource::read_template(PointIterator write_iterator, size_t n) {
    CartesianRecord<float> record;
    double shift = -box_size/2.;

    size_t loaded = 0;
    size_t to_load = std::min(n, n_points - current_point);
    if(to_load == 0)
      return -1;  // EOF

    // Before this loop, reader is guaranteed to be in a readable position (at least one element)
    while(loaded < to_load) {
      fd.read((char *) &record, sizeof(record));
      *write_iterator = record.point(shift);

      // Increment tracking variables
      loaded++;
      write_iterator++;
      current_point++;
      current_point_file++;

      // If at end of file, load new file
      // or break out of loop if nothing left to read
      if(current_point_file == n_points_file) {
        if(current_file == files - 1) {
          if(loaded < to_load)
            std::cout << "WARNING: Did not find as many points as expected ("
                      << current_point << " instead of " << n_points << ")." << std::endl;
          break;
        } else {
          load_file(++current_file);
        }
      }
    }
    return loaded;
  }

  long long int GadgetSource::read(PointContainer::iterator write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  long long int GadgetSource::read(Point *write_iterator, size_t n) {
    return read_template(write_iterator, n);
  }

  size_t GadgetSource::get_npoints() {
    return n_points;
  }

  void GadgetSource::load_file(std::string fname, bool initial_file) {
    if(fd.is_open())
      fd.close();
    io_header header;

    // Open file at the end
    fd.open(fname, std::ios::in | std::ios::binary | std::ios::ate);
    if(!fd.is_open()) {
      std::cout << "WARNING: Could not open file " << fname << std::endl;
    } else {
      if(verbose)
        std::cout << "Opened file " << fname << std::endl;
      // Current position == size
      std::streampos size = fd.tellg();

      // Move to front, skip first 4bytes
      fd.seekg(skipsize, std::ios::beg);
      // Read header
      fd.read((char *) &header, sizeof(header));
      fd.seekg(skipsize, std::ios::cur);
      // Skip gas particles
      fd.seekg(header.npart[0] * sizeof(float) * 3, std::ios::cur);
      fd.seekg(skipsize, std::ios::cur);
      // Now we should be at the correct position (Halo Particles)

      // Set class variables from header (current file stats)
      current_point_file = 0;
      n_points_file = header.npart[1];
      current_file = (unsigned int) stoi(fname.substr(fname.find_last_of('.') + 1, std::string::npos));

      // Assert that file is large enough
      std::streampos avail_size = size - fd.tellg();
      assert(avail_size >= 3 * sizeof(float) * n_points_file);

      // If initial_file, also read overall stats
      if(initial_file) {
        files = (unsigned int) header.num_files;
        n_points = (unsigned int) header.npartTotal[1];

        //Cosmo Parameters
        hubble_parameter = header.HubbleParam;
        box_size = header.BoxSize;
      }
    }
  }

  void GadgetSource::load_file(unsigned int filenum) {
    std::string fname = filename + '.' + std::to_string(filenum);
    load_file(fname);
  }

  void GadgetSource::reset() {
    if(files > 1) {
      load_file(0);
    } else {
      load_file(filename);
    }
    current_point = 0;
  }

}}