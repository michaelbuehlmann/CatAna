#include <catana/points/Point.hpp>


namespace catana {
  Point::Point(const double& x, const double& y, const double& z) {
    vec3 vec(x, y, z);
    r = vec.Length();
    p = pointing(vec);
    p.normalize();
  }

  Point point_from_box_position(const double& pos_x, const double& pos_y, const double& pos_z,
                                const double& shift, const double& hubble_param) {
    Point obj(
        (pos_x + shift),
        (pos_y + shift),
        (pos_z + shift)
    );
    obj.r /= hubble_param;
    return obj;
  }

  Point point_from_spherical_position(const double& pos_r, const double& pos_theta,
                                      const double& pos_phi, const double& hubble_param) {
    Point obj;
    obj.r = pos_r / hubble_param;
    obj.p = pointing(pos_theta, pos_phi);
    return obj;
  }


}