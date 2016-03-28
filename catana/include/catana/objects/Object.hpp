//
// Created by Michael Bühlmann on 02/02/16.
//

#ifndef CATANA_APP_OBJECT_TYPES_HPP
#define CATANA_APP_OBJECT_TYPES_HPP

#include <pointing.h>

namespace catana {

    //! Catalog Object with position in 3d space
    struct Object {
        /*
         * Representation of an object in the catalog.
         * Radius r>0
         * angular position p (HEALPix pointing)
         */
        Object() = default;
        Object(const double& x, const double& y, const double& z);

        double r;
        pointing p;
    };

    //! Create Object from box coordinates (coordinates in [0, boxsize]). Change coordinates from Mpc/h to Mpc.
    //  set boxsize=0 if center of box already at (0,0,0)
    Object object_from_box_position(const double& pos_x, const double& pos_y, const double& pos_z,
            const double& boxsize = 0, const double& hubble_param = 1);


    template<class FLOAT_TYPE>
    Object object_from_box_position(const FLOAT_TYPE* pos_xyz, const double& boxsize = 0,
            const double& hubble_param = 1)
    {
        return object_from_box_position(pos_xyz[0], pos_xyz[1], pos_xyz[2], boxsize, hubble_param);
    }


    Object object_from_spherical_position(const double& pos_r, const double& pos_theta,
            const double& pos_phi, const double& hubble_param = 1);


    template<class FLOAT_TYPE>
    Object object_from_spherical_position(const FLOAT_TYPE* pos_rtf, const double& hubble_param = 1)
    {
        return object_from_spherical_position(pos_rtf[0], pos_rtf[1], pos_rtf[2], hubble_param);
    }

}


#endif //CATANA_APP_OBJECT_TYPES_HPP
