#include <catana/catana.hpp>

#include <iomanip>
#include "timer.hpp"


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

void print_arguments() {
  std::cout
      << "Arguments: lmax nmax nside parallel [True/False] interpolated [True/False] method [raw/reverse/reverse_fft] obj_number"
      << std::endl;
}

enum class Method {
  RAW,
  REVERSE,
  REVERSE_FFT
};

struct RunArgs {
  int lmax;
  int nmax;
  int nside;
  int n;
  bool parallel;
  bool interpolated;
  Method method;
};

RunArgs parser_arguments(int argc, char *argv[]) {
  if(argc != 8) {
    std::cout << "Got " << argc << " arguments." << std::endl;
    print_arguments();
    throw "ARGUMENT_ERROR";
  }

  RunArgs run_args;

  run_args.lmax = std::stoi(argv[1]);
  run_args.nmax = std::stoi(argv[2]);
  run_args.nside = std::stoi(argv[3]);

  if(std::string(argv[4]) == std::string("True"))
    run_args.parallel = true;
  else if(std::string(argv[4]) == std::string("False"))
    run_args.parallel = false;
  else {
    std::cout << "Parallel either 'True' or 'False' (got '" << argv[4] << "')" << std::endl;
    throw "ARGUMENT_ERROR";
  }

  if(std::string(argv[5]) == std::string("True"))
    run_args.interpolated = true;
  else if(std::string(argv[5]) == std::string("False"))
    run_args.interpolated = false;
  else {
    std::cout << "Interpolated either 'True' or 'False' (got '" << argv[5] << "')" << std::endl;
    throw "ARGUMENT_ERROR";
  }

  if(std::string(argv[6]) == std::string("raw")) {
    run_args.method = Method::RAW;
  } else if(std::string(argv[6]) == std::string("reverse")) {
    run_args.method = Method::REVERSE;
  } else if(std::string(argv[6]) == std::string("reverse_fft")) {
    run_args.method = Method::REVERSE_FFT;
  } else {
    std::cout << "Unknown method: " << argv[6] << std::endl;
    print_arguments();
    throw "ARGUMENT_ERROR";
  }

  run_args.n = std::stoi(argv[7]);

  return run_args;
}


int main(int argc, char *argv[]) {
  RunArgs run_args(parser_arguments(argc, argv));
  timer::Timer timer;

  timer.start();

  int threads = 1;
#if defined(_OPENMP)
#pragma omp parallel if(run_args.parallel)
  {
#pragma omp master
    {
      threads = omp_get_num_threads();
      std::cout << "# Using OpenMP with " << threads << " threads of maximal " << omp_get_max_threads() << std::endl;
    }
  }
#else
  std::cout << "# Not compiled with OpenMP support. Using 1 thread only." << std::endl;
#endif // defined(_OPENMP)

  double box_size = 100;

  double internal_runs = 300. / std::sqrt(run_args.n);
  internal_runs *= (20. / run_args.lmax);
  internal_runs *= (50. / run_args.nmax);
  internal_runs *= threads;
  if(run_args.interpolated) {
    internal_runs *= 5;
  }
  internal_runs = std::max(internal_runs, 1.);

  std::cout << "# Within tophat: " << run_args.n << " points. Number of runs to average time over: "
            << int(internal_runs) << std::endl;
  std::cout << "# Analyzing method: " << argv[6] << std::endl;

  KClkk kclkk(run_args.lmax, run_args.nmax, box_size / 2.);

  std::stringstream output_filename;
  output_filename << run_args.lmax << run_args.nmax << run_args.nside << run_args.parallel << run_args.interpolated
                  << run_args.n << threads << "_";
  switch(run_args.method) {
    case Method::RAW:
      output_filename << "raw";
      break;
    case Method::REVERSE:
      output_filename << "reverse";
      break;
    case Method::REVERSE_FFT:
      output_filename << "reverse_fft";
      break;
  }

  PointContainer oc(random_points(run_args.n, box_size));
  PixelizedPointContainer pix_oc(run_args.nside, oc);
  timer.stop();
  std::cout << "# Time for pointcontainer creation: " << timer.duration() << std::endl;

  timer.start();
  for(int i = 0; i < internal_runs; ++i) {
    switch(run_args.method) {
      case Method::RAW:
        kclkk = _sfb_raw(oc, run_args.lmax, run_args.nmax, box_size / 2., false, false,
                         run_args.parallel, run_args.interpolated);
        break;
      case Method::REVERSE:
        kclkk = _sfb_reverse(pix_oc, run_args.lmax, run_args.nmax, box_size / 2., false, false,
                             run_args.parallel, run_args.interpolated);
        break;
      case Method::REVERSE_FFT:
        kclkk = _sfb_reverse_fft(pix_oc, run_args.lmax, run_args.nmax, box_size / 2., false, false,
                                 run_args.parallel, run_args.interpolated);
        break;
    }
  }
  timer.stop();
  double time = timer.duration() / int(internal_runs);

  kclkk.savetxt(output_filename.str());

  std::cout << std::endl;
  std::cout << std::setw(12) << "# ___size___" << " " << "___Avg time___" << std::endl;
  std::cout << std::setw(12) << oc.size() << " " << std::setprecision(8) << std::scientific << time << std::endl;
  return 0;
}

