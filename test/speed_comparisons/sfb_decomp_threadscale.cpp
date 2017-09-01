#include <catana/catana.hpp>
#include "timer.hpp"

#include <unistd.h>


using namespace catana;

PointContainer random_points(size_t n, double box_size) {
  PointContainer oc(n);
  io::TophatRadialWindowFunctionFilter tophat_filter(box_size / 2.);

#pragma omp parallel
  {
    std::mt19937 rng_private;
    std::uniform_real_distribution<double> dist(-box_size / 2., box_size / 2.);
    Point point;
#pragma omp for
    for(size_t i = 0; i < n; ++i) {
      do {
        point = Point(dist(rng_private), dist(rng_private), dist(rng_private));
      } while(!tophat_filter.filter(point));
      oc[i] = point;
    }
  }
  return oc;
}

int main(int argc, char *argv[]) {
  unsigned short lmax = 50;
  unsigned short nmax = 100;
  unsigned short nside = 256;
  size_t n = 4000000;
  double box_size = 1000;

  PointContainer oc = random_points(n, box_size);
  PixelizedPointContainer poc(nside, oc);

  Timer timer;

  for(int i = 1; i < argc; ++i) {
    int nthreads = std::stoi(argv[i]);
    omp_set_num_threads(nthreads);
    int runs = std::min(nthreads, 10);

    timer.start();
    for(int j = 0; j < runs; ++j) {
      auto kclkk = _sfb_reverse_fft(poc, lmax, nmax, 0.5*box_size, false, false, true, true);
    }
    timer.stop();
    double duration = timer.duration_ms() / nthreads;
    std::cout << nthreads << " " << duration << std::endl;
  }

  return 0;
}