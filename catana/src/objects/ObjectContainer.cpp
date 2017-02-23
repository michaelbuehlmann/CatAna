#include <catana/objects/ObjectContainer.hpp>
#include <random>
#include <cassert>




namespace catana {
  extern std::mt19937 rng;
  void create_object_container_subset(ObjectContainer& object_container, const size_t& n) {
    assert(n < object_container.size());
    std::uniform_int_distribution<size_t> dist;  // default: (0, max(numeric type)

    auto current = object_container.begin();
    size_t obj_remaining = object_container.size();
    size_t obj_toadd = n;

    while(obj_toadd--) {
      auto random_pick = current + (dist(rng) % obj_remaining);
      std::swap(*current, *random_pick);
      ++current;
      --obj_remaining;
    }
    object_container.resize(n);
  }

}