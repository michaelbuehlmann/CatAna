//
// Created by Michael Bühlmann on 02/02/16.
//

#ifndef CATANA_APP_OBJECT_TYPES_HPP
#define CATANA_APP_OBJECT_TYPES_HPP

#include <pointing.h>

namespace catana {

    //! Representation of a particle in 3-dimensional space in spherical coordinates
    struct Object {
        //! Default constructor
        Object() = default;

        //! Construct from cartesian coordinates
        Object(const double& x, const double& y, const double& z);

        double r;   //!< radius r>0
        pointing p; //!< angular position p (HEALPix pointing)
    };


    //! Create Object from box coordinates (coordinates in [0, boxsize]). Change coordinates from Mpc/h to Mpc.
    /*!
     * @param pos_x x-coordinate in [0, boxsize]
     * @param pos_y y-coordinate in [0, boxsize]
     * @param pos_z z-coordinate in [0, boxsize]
     * @param boxsize will be used to shift origin to center of the box (i.e. coordinates will be transformed x ->
     * x-box_size/2.). If origin already located at center of box, set box_size=0.
     * @param hubble_param The Hubble parameter h which will be used to convert Mpc/h -> Mpc. If coordinates are already
     * in Mpc, set h=1.
     */
    Object object_from_box_position(const double& pos_x, const double& pos_y, const double& pos_z,
            const double& boxsize = 0, const double& hubble_param = 1);


    //! Create Object from box coordinates (coordinates in [0, boxsize]). Change coordinates from Mpc/h to Mpc.
    /*!
     * @tparam FLOAT_TYPE the floating point type of the pos_xyz array (float or double)
     * @param pos_xyz pointer to C-array of type FLOAT_TYPE with size 3, containing the x, y, z coordinates
     * @param boxsize will be used to shift origin to center of the box (i.e. coordinates will be transformed x ->
     * x-box_size/2.). If origin already located at center of box, set box_size=0.
     * @param hubble_param The Hubble parameter h which will be used to convert Mpc/h -> Mpc. If coordinates are already
     * in Mpc, set h=1.
     */
    template<class FLOAT_TYPE>
    Object object_from_box_position(const FLOAT_TYPE* pos_xyz, const double& boxsize = 0,
            const double& hubble_param = 1)
    {
        return object_from_box_position(pos_xyz[0], pos_xyz[1], pos_xyz[2], boxsize, hubble_param);
    }


    //! Create Object from box spherical coordinates. Change coordinates from Mpc/h to Mpc.
    /*!
     * @param pos_r     radial coordinate pos_r > 0
     * @param pos_theta lattitude coordinate 0 <= pos_theta <= pi
     * @param pos_phi   longitude coordinate 0 <= pos_phi <= 2pi
     * @param hubble_param The Hubble parameter h which will be used to convert Mpc/h -> Mpc. If coordinates are already
     * in Mpc, set h=1.
     */
    Object object_from_spherical_position(const double& pos_r, const double& pos_theta,
            const double& pos_phi, const double& hubble_param = 1);

    //! Create Object from box spherical coordinates. Change coordinates from Mpc/h to Mpc.
    /*!
     * @tparam FLOAT_TYPE the floating point type of the pos_rtf array (float or double)
     * @param pos_rtf pointer to C-array of type FLOAT_TYPE with size 3, containing the r, theta, phi coordinates
     * @param hubble_param The Hubble parameter h which will be used to convert Mpc/h -> Mpc. If coordinates are already
     * in Mpc, set h=1.
     */
    template<class FLOAT_TYPE>
    Object object_from_spherical_position(const FLOAT_TYPE* pos_rtf, const double& hubble_param = 1)
    {
        return object_from_spherical_position(pos_rtf[0], pos_rtf[1], pos_rtf[2], hubble_param);
    }

}


#endif //CATANA_APP_OBJECT_TYPES_HPP
