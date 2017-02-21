#include <catana/objects/PixelizedObjectContainer.hpp>
#include <numeric>


namespace catana {

  bool is_power_of_2(const int& n) {
    return (((n & (n - 1)) == 0));
  }

  PixelizedObjectContainer::PixelizedObjectContainer(unsigned int nside)
      : hp_base(nside, RING, SET_NSIDE), nside(nside) {
    if(!is_power_of_2(nside)) {
      throw std::invalid_argument("argument (nside) must be a power of 2");
    }
    resize(hp_base.Npix());
    for(int i = 0; i < hp_base.Npix(); ++i) {
      operator[](i).p = hp_base.pix2ang(i);
    }
  }

  PixelizedObjectContainer::PixelizedObjectContainer(unsigned int nside, const ObjectContainer& oc)
      : PixelizedObjectContainer(nside) {
    std::for_each(oc.begin(), oc.end(), [this](const Object& object) { this->add_object(object); });
  }

  void PixelizedObjectContainer::add_object(const Object& object) {
    int i = hp_base.ang2pix(object.p);
    operator[](i).push_back(object.r);
  }

  unsigned int PixelizedObjectContainer::get_nside() const {
    return nside;
  }

  size_t PixelizedObjectContainer::get_nobjects() const {
    return std::accumulate(begin(), end(), 0ULL,
                           [&](const long sum, const PixelObjects& pix_objs) { return sum + pix_objs.size(); });
  }

  const Healpix_Base& PixelizedObjectContainer::get_hp_base() const {
    return hp_base;
  }

  Eigen::ArrayXi PixelizedObjectContainer::get_countmap() const {
    int npix = hp_base.Npix();
    Eigen::ArrayXi map = Eigen::ArrayXi::Zero(npix);
    for(int i = 0; i < npix; ++i) {
      map(i) = operator[](i).size();
    }
    return map;
  }

}