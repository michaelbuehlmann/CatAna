#ifndef CATANA_APP_RECORD_TYPE_CPP_HPP
#define CATANA_APP_RECORD_TYPE_CPP_HPP

#include "catana/io/record_types.hpp"


namespace catana { namespace io {

  template<class FLOAT_TYPE>
  std::ostream& operator<<(std::ostream& os, const CartesianRecord<FLOAT_TYPE>& rec) {
    os << rec.pos_x << " " << rec.pos_y << " " << rec.pos_z;
    return os;
  }
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////////////
  // SPHERICAL RECORD BASE
  template<class FLOAT_TYPE>
  SphericalRecordBase<FLOAT_TYPE>::SphericalRecordBase()
      :pos_r(0), pos_theta(0), pos_phi(0) {}

  template<class FLOAT_TYPE>
  SphericalRecordBase<FLOAT_TYPE>::SphericalRecordBase(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi)
      :pos_r(r), pos_theta(theta), pos_phi(phi) {}

  template<class FLOAT_TYPE>
  SphericalRecordBase<FLOAT_TYPE>::SphericalRecordBase(const Point& point, const double& shift)
      :pos_r(point.r), pos_theta(point.p.theta), pos_phi(point.p.phi) {}

  template<class FLOAT_TYPE>
  Point SphericalRecordBase<FLOAT_TYPE>::point(const double& shift) {
    return point_from_spherical_position(pos_r, pos_theta, pos_phi);
  }

  ///////////////////////////////////////////////////////////////////////////////
  // SPHERICAL RECORD

  template<class FLOAT_TYPE, SphericalTextFormat FMT>
  SphericalRecord<FLOAT_TYPE, FMT>::SphericalRecord()
      :SphericalRecordBase<FLOAT_TYPE>() {}

  template<class FLOAT_TYPE, SphericalTextFormat FMT>
  SphericalRecord<FLOAT_TYPE, FMT>::SphericalRecord(FLOAT_TYPE r, FLOAT_TYPE theta, FLOAT_TYPE phi)
      :SphericalRecordBase<FLOAT_TYPE>(r, theta, phi) {}

  template<class FLOAT_TYPE, SphericalTextFormat FMT>
  SphericalRecord<FLOAT_TYPE, FMT>::SphericalRecord(const Point& point, const double& dummy)
      :SphericalRecordBase<FLOAT_TYPE>(point, dummy) {}

  template<class FLOAT_TYPE>
  SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>::SphericalRecord()
      :SphericalRecordBase<FLOAT_TYPE>() {}

  template<class FLOAT_TYPE>
  SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>::SphericalRecord(FLOAT_TYPE phi, FLOAT_TYPE theta,
                                                                              FLOAT_TYPE r)
      :SphericalRecordBase<FLOAT_TYPE>(r, theta, phi) {}

  template<class FLOAT_TYPE>
  SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>::SphericalRecord(const Point& point, const double& shift)
      :SphericalRecordBase<FLOAT_TYPE>(point, shift) {}

  template<class FLOAT_TYPE, SphericalTextFormat FMT>
  std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, FMT>& rec) {
    os << rec.pos_r << " " << rec.pos_theta << " " << rec.pos_phi;
    return os;
  }

  template<class FLOAT_TYPE>
  std::ostream& operator<<(std::ostream& os, const SphericalRecord<FLOAT_TYPE, SphericalTextFormat::THREEDEX>& rec) {
    char buffer[60];
    sprintf(buffer, " %18.14F %18.14F %18.12F", rec.pos_phi, rec.pos_theta, rec.pos_r);
    return os << buffer;
  }
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////
  // IMPLICIT INSTANTIATION (add needed cases)
  template
  struct CartesianRecord<float>;
  template
  struct CartesianRecord<double>;
  template
  struct SphericalRecordBase<float>;
  template
  struct SphericalRecordBase<double>;
  template
  struct SphericalRecord<float, SphericalTextFormat::RTHETAPHI>;
  template
  struct SphericalRecord<double, SphericalTextFormat::RTHETAPHI>;
  template
  struct SphericalRecord<float, SphericalTextFormat::THREEDEX>;
  template
  struct SphericalRecord<double, SphericalTextFormat::THREEDEX>;

  template std::ostream& operator<<<float>(std::ostream&, const CartesianRecord<float>&);

  template std::ostream& operator<<<double>(std::ostream&, const CartesianRecord<double>&);

  template std::ostream& operator<<<float, SphericalTextFormat::RTHETAPHI>(std::ostream&,
                                                                           const SphericalRecord<float, SphericalTextFormat::RTHETAPHI>&);

  template std::ostream& operator<<<double, SphericalTextFormat::RTHETAPHI>(std::ostream&,
                                                                            const SphericalRecord<double, SphericalTextFormat::RTHETAPHI>&);

  template std::ostream& operator<<<float>(std::ostream&, const SphericalRecord<float, SphericalTextFormat::THREEDEX>&);

  template std::ostream&
  operator<<<double>(std::ostream&, const SphericalRecord<double, SphericalTextFormat::THREEDEX>&);
}}

#endif //CATANA_APP_RECORD_TYPE_CPP_HPP