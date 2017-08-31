#ifndef CATANA_APP_POINT_TYPES_HPP
#define CATANA_APP_POINT_TYPES_HPP

#include <pointing.h>


namespace catana {

  //! Representation of a particle in 3-dimensional space in spherical coordinates
  struct Point {
    //! Default constructor
    Point();

    //! Construct from cartesian coordinates
    Point(const double& x, const double& y, const double& z);

    double r;   //!< radius r>0
    pointing p; //!< angular position p (HEALPix pointing)
  };


  //! Create Point from box coordinates (coordinates in [0, boxsize]). Change coordinates from Mpc/h to Mpc.
  /*!
   * @param pos_x x-coordinate in [0, boxsize]
   * @param pos_y y-coordinate in [0, boxsize]
   * @param pos_z z-coordinate in [0, boxsize]
   * @param shift will be used to shift origin to center of the box (i.e. coordinates will be transformed x ->
   * x+shift). If origin already located at center of box, set shift=0.
   */
  Point point_from_box_position(const double& pos_x, const double& pos_y, const double& pos_z, const double& shift = 0);


  //! Create Point from box coordinates (coordinates in [0, boxsize]). Change coordinates from Mpc/h to Mpc.
  /*!
   * @tparam FLOAT_TYPE the floating point type of the pos_xyz array (float or double)
   * @param pos_xyz pointer to C-array of type FLOAT_TYPE with size 3, containing the x, y, z coordinates
   * @param shift will be used to shift origin to center of the box (i.e. coordinates will be transformed x ->
   * x+shift). If origin already located at center of box, set shift=0.
   */
  template<class FLOAT_TYPE>
  Point point_from_box_position(const FLOAT_TYPE *pos_xyz, const double& shift = 0) {
    return point_from_box_position(pos_xyz[0], pos_xyz[1], pos_xyz[2], shift);
  }


  //! Create Point from box spherical coordinates. Change coordinates from Mpc/h to Mpc.
  /*!
   * @param pos_r     radial coordinate pos_r > 0
   * @param pos_theta lattitude coordinate 0 <= pos_theta <= pi
   * @param pos_phi   longitude coordinate 0 <= pos_phi <= 2pi
   */
  Point point_from_spherical_position(const double& pos_r, const double& pos_theta, const double& pos_phi);

  //! Create Point from box spherical coordinates. Change coordinates from Mpc/h to Mpc.
  /*!
   * @tparam FLOAT_TYPE the floating point type of the pos_rtf array (float or double)
   * @param pos_rtf pointer to C-array of type FLOAT_TYPE with size 3, containing the r, theta, phi coordinates
   */
  template<class FLOAT_TYPE>
  Point point_from_spherical_position(const FLOAT_TYPE *pos_rtf) {
    return point_from_spherical_position(pos_rtf[0], pos_rtf[1], pos_rtf[2]);
  }
}


#endif //CATANA_APP_POINT_TYPES_HPP
