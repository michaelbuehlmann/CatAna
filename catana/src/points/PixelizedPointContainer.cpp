#include <catana/points/PixelizedPointContainer.hpp>
#include <numeric>


namespace catana {

  bool is_power_of_2(const int& n) {
    return (((n & (n - 1)) == 0));
  }

  PixelizedPointContainer::PixelizedPointContainer(unsigned int nside)
      : hp_base(nside, RING, SET_NSIDE), nside(nside) {
    if(!is_power_of_2(nside)) {
      throw std::invalid_argument("argument (nside) must be a power of 2");
    }
    resize(hp_base.Npix());
    for(int i = 0; i < hp_base.Npix(); ++i) {
      operator[](i).p = hp_base.pix2ang(i);
    }
  }

  PixelizedPointContainer::PixelizedPointContainer(unsigned int nside, const PointContainer& oc)
      : PixelizedPointContainer(nside) {
    std::for_each(oc.begin(), oc.end(), [this](const Point& point) { this->add_point(point); });
  }

  void PixelizedPointContainer::add_point(const Point& point) {
    int i = hp_base.ang2pix(point.p);
    operator[](i).push_back(point.r);
  }

  unsigned int PixelizedPointContainer::get_nside() const {
    return nside;
  }

  size_t PixelizedPointContainer::get_npoints() const {
    return std::accumulate(begin(), end(), 0ULL,
                           [&](const long sum, const PixelPoints& pix_objs) { return sum + pix_objs.size(); });
  }

  const Healpix_Base& PixelizedPointContainer::get_hp_base() const {
    return hp_base;
  }

  Eigen::ArrayXi PixelizedPointContainer::get_countmap() const {
    int npix = hp_base.Npix();
    Eigen::ArrayXi map = Eigen::ArrayXi::Zero(npix);
    for(int i = 0; i < npix; ++i) {
      map(i) = operator[](i).size();
    }
    return map;
  }

}