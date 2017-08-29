//! \file record_types.hpp
//! \brief The record classes defined here map out the memory layout of binary files (Gadget, RawBinary) and ordering in Text Files
/*! \file
 * All classes must define a member function  which returns a Point
 * and a constructor from an Point and box_size, hubble_param.
 *
 * Add these members:
 *      CartesianRecord()
 *      CartesianRecord(const Point&, const double& box_size, const double& hubble_param)
 *      Point point(box_size, hubble_param)
 */


#ifndef CATANA_APP_RECORD_TYPES_HPP
#define CATANA_APP_RECORD_TYPES_HPP

#include "../points/Point.hpp"

// TODO: FIX BOX_SIZE (assume box is always [0, box_size]

namespace catana { namespace io {

  //! Cartesian Point storage.
  /*!
   * @tparam FLOAT_TYPE the floating point type used to store the coordinates (either float or double)
   */
  template<class FLOAT_TYPE>
  struct CartesianRecord {
    typedef FLOAT_TYPE float_t; //!< the floating point type used to store record
    FLOAT_TYPE pos_x;           //!< x-coordinate
    FLOAT_TYPE pos_y;           //!< y-coordinate
    FLOAT_TYPE pos_z;           //!< z-coordinate

    //! Default Construct: initialize to 0
    CartesianRecord()
        : pos_x(0), pos_y(0), pos_z(0) {}

    //! Construct from cartesian coordinates
    CartesianRecord(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z)
        : pos_x(x), pos_y(y), pos_z(z) {}

    //! Construct from Point
    /*!
     * @param point
     * The point from which the record is constructed
     * @param box_size
     * Add box_size/2 to coordinates (if origin of coordinate system shall be at corner of box/cube)
     * If origin is supposed to be at the center, set box_size=0.
     * @param hubble_param
     * Hubble parameter for conversion Mpc -> Mpc/h. If record coordinates are Mpc, set hubble_param=1.
     */
    CartesianRecord(const Point& point, const double& box_size, const double& hubble_param) {
      pos_x = pos_y = pos_z = point.r * hubble_param;
      vec3 unit_vec(point.p.to_vec3());
      pos_x *= unit_vec.x;
      pos_y *= unit_vec.y;
      pos_z *= unit_vec.z;
      pos_x += 0.5 * box_size;
      pos_y += 0.5 * box_size;
      pos_z += 0.5 * box_size;
    }

    //! Construct Point from record
    /*!
     * @param box_size
     * Subtract box_size/2 from coordinates (if origin is located at corner of box). If coordinates are already
     * centered at box-center, set box_size=0.
     * @param hubble_param
     * Hubble parameter for conversion Mpc/h -> Mpc. If record coordinates are Mpc, set hubble_param=1.
     */
    Point point(const double& box_size, const double& hubble_param) {
      return point_from_box_position(pos_x, pos_y, pos_z, box_size, hubble_param);
    }
  };


  //! Ordering types of SphericalRecord
  enum class SphericalTextFormat {
    //! Standard order: radius, theta (latitude, [0,pi]), phi (longitude, [0,2pi])
        RTHETAPHI,
    //! 3DEX order: phi (longitude, [0,2pi]), theta (latitude, [0,pi]), radius
        THREEDEX
  };


  //! Base class for SphericalRecord. Do not use this directly
  /*!
   * @tparam FLOAT_TYPE the floating point type used to store the coordinates (either float or double)
   */
  template<class FLOAT_TYPE>
  struct SphericalRecordBase {
    typedef FLOAT_TYPE float_t;  //!< the floating point type used to store record
    FLOAT_TYPE pos_r;            //!< radial coordinate
    FLOAT_TYPE pos_theta;        //!< latitude coordinate [0, pi]
    FLOAT_TYPE pos_phi;          //!< longitude coordinate [0, 2pi]

    //! Default construct
    SphericalRecordBase();

    //! Construct from spherical coordinates
    SphericalRecordBase(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi);

    //! Construct from Point
    /*!
    * @param point
    * The point from which the record is constructed
    * @param box_size
    * Add box_size/2 to coordinates (if origin of coordinate system shall be at corner of box/cube)
    * If origin is supposed to be at the center, set box_size=0.
    * @param hubble_param
    * Hubble parameter for conversion Mpc -> Mpc/h. If record coordinates are Mpc, set hubble_param=1.
    */
    SphericalRecordBase(const Point& point, const double& box_size, const double& hubble_param);

    //! Construct Point from record
    /*!
     * @param box_size
     * Subtract box_size/2 from coordinates (if origin is located at corner of box). If coordinates are already
     * centered at box-center, set box_size=0.
     * @param hubble_param
     * Hubble parameter for conversion Mpc/h -> Mpc. If record coordinates are Mpc, set hubble_param=1.
     */
    Point point(const double& box_size, const double& hubble_param);
  };


  //! Spherical Point storage
  /*!
   * @tparam FLOAT_TYPE the floating point type used to store the coordinates (either float or double)
   * @tparam FMT SphericalTextFormat for TextSource, TextSink interface
   */
  template<class FLOAT_TYPE, SphericalTextFormat FMT = SphericalTextFormat::RTHETAPHI>
  struct SphericalRecord : public SphericalRecordBase<FLOAT_TYPE> {

    //! Default construct
    SphericalRecord();

    //! Construct from spherical coordinates
    SphericalRecord(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi);

    //! Construct from Point
    /*!
    * @param point
    * The point from which the record is constructed
    * @param box_size
    * Add box_size/2 to coordinates (if origin of coordinate system shall be at corner of box/cube)
    * If origin is supposed to be at the center, set box_size=0.
    * @param hubble_param
    * Hubble parameter for conversion Mpc -> Mpc/h. If record coordinates are Mpc, set hubble_param=1.
    */
    SphericalRecord(const Point& point, const double& box_size, const double& hubble_param);
  };


  //! Spherical Point storage, specialization for THREEDEX SphericalTextFormat.
  /*!
   * @tparam FLOAT_TYPE the floating point type used to store the coordinates (either float or double)
   */
  template<class FLOAT_TYPE>
  struct SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX> : public SphericalRecordBase<FLOAT_TYPE> {
    SphericalRecord();

    //! Construct from spherical coordinates
    SphericalRecord(FLOAT_TYPE theta, FLOAT_TYPE phi, FLOAT_TYPE r);

    //! Construct from POint
    /*!
    * @param point
    * The point from which the record is constructed
    * @param box_size
    * Add box_size/2 to coordinates (if origin of coordinate system shall be at corner of box/cube)
    * If origin is supposed to be at the center, set box_size=0.
    * @param hubble_param
    * Hubble parameter for conversion Mpc -> Mpc/h. If record coordinates are Mpc, set hubble_param=1.
    */
    SphericalRecord(const Point& point, const double& box_size, const double& hubble_param);
  };


  //! Outstream for CartesianRecord (used for TextSink)
  template<class FLOAT_TYPE>
  std::ostream& operator<<(std::ostream& os, const CartesianRecord<FLOAT_TYPE>& rec);

  //! Outstream for SphericalRecord, standard order (used for TextSink)
  template<class FLOAT_TYPE, SphericalTextFormat FMT>
  std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, FMT>& rec);

  //! Outstream for SphericalRecord, 3DEX order (used for TextSink)
  template<class FLOAT_TYPE>
  std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>& rec);

}}
#endif //CATANA_APP_RECORD_TYPES_HPP
