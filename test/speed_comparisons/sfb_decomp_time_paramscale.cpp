#include <catana/catana.hpp>
#include "timer.hpp"
#include "json.hpp"
#include <stdexcept>


using namespace catana;
using json = nlohmann::json;

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

enum ComputeMethod {
  REV = 0b0001,
  RAW = 0b0010,
  REV_NOFFT = 0b0100
};

enum Interp {
  NO = 0b01,
  YES = 0b10
};

struct StepArgs {
  unsigned short lmax;
  unsigned short nmax;
  unsigned short nside;
  unsigned short nruns;
  unsigned int n;
  char compute_method;
  char interp_method;
};

std::ostream& operator<<(std::ostream& os, const StepArgs args) {
  os << "n: " << args.n << ", lmax: " << args.lmax << ", nmax: " << args.nmax << ", ";
  os << "nside: " << args.nside << ", nruns:" << args.nruns << ", ";
  os << "cm: " << int(args.compute_method) << ", pm: " << int(args.interp_method);
  return os;
}

std::string execute_step(const StepArgs& args) {
  std::cout << "executing step with params " << args << std::endl;
  double box_size = 100;
  auto pc = random_points(args.n, box_size);
  PixelizedPointContainer ppc(args.nside, pc);
  std::cout << "\tcontainers built" << std::endl;

  std::stringstream out;
  out << args.n << " " << args.lmax << " " << args.nmax << " " << args.nside << " " << args.nruns << " ";

  Timer timer;

  // RAW
  if(args.compute_method & ComputeMethod::RAW) {
    std::cout << "\tdoing RAW" << std::endl;
    if(args.interp_method & Interp::YES) {
      std::cout << "\t\twith interpolation" << std::endl;
      timer.start();
      for(unsigned i = 0; i < args.nruns; ++i) {
        auto kclkk = _sfb_raw(pc, args.lmax, args.nmax, box_size / 2, false, false, false, true);
      }
      timer.stop();
      std::cout << "\t\t took " << timer.duration_ms()/args.nruns << "ms" << std::endl;
      out << timer.duration_ms()/args.nruns << " ";
    }
    if(args.interp_method & Interp::NO) {
      std::cout << "\t\twithout interpolation" << std::endl;
      timer.start();
      for(unsigned i = 0; i < args.nruns; ++i) {
        auto kclkk = _sfb_raw(pc, args.lmax, args.nmax, box_size / 2, false, false, false, false);
      }
      timer.stop();
      std::cout << "\t\t took " << timer.duration_ms()/args.nruns << "ms" << std::endl;
      out << timer.duration_ms()/args.nruns << " ";
    }
  }

  // REV
  if(args.compute_method & ComputeMethod::REV) {
    std::cout << "\tdoing REV" << std::endl;
    if(args.interp_method & Interp::YES) {
      std::cout << "\t\twith interpolation" << std::endl;
      timer.start();
      for(unsigned i = 0; i < args.nruns; ++i) {
        auto kclkk = _sfb_reverse_fft(ppc, args.lmax, args.nmax, box_size / 2, false, false, false, true);
      }
      timer.stop();
      std::cout << "\t\t took " << timer.duration_ms()/args.nruns << "ms" << std::endl;
      out << timer.duration_ms()/args.nruns << " ";
    }
    if(args.interp_method & Interp::NO) {
      std::cout << "\t\twithout interpolation" << std::endl;
      timer.start();
      for(unsigned i = 0; i < args.nruns; ++i) {
        auto kclkk = _sfb_reverse_fft(ppc, args.lmax, args.nmax, box_size / 2, false, false, false, false);
      }
      timer.stop();
      std::cout << "\t\t took " << timer.duration_ms()/args.nruns << "ms" << std::endl;
      out << timer.duration_ms()/args.nruns << " ";
    }
  }

  // REV (NO FFT)
  if(args.compute_method & ComputeMethod::REV_NOFFT) {
    std::cout << "\tdoing REV (NO FFT)" << std::endl;
    if(args.interp_method & Interp::YES) {
      std::cout << "\t\twith interpolation" << std::endl;
      timer.start();
      for(unsigned i = 0; i < args.nruns; ++i) {
        auto kclkk = _sfb_reverse(ppc, args.lmax, args.nmax, box_size / 2, false, false, false, true);
      }
      timer.stop();
      std::cout << "\t\t took " << timer.duration_ms()/args.nruns << "ms" << std::endl;
      out << timer.duration_ms()/args.nruns << " ";
    }
    if(args.interp_method & Interp::NO) {
      std::cout << "\t\twithout interpolation" << std::endl;
      timer.start();
      for(unsigned i = 0; i < args.nruns; ++i) {
        auto kclkk = _sfb_reverse(ppc, args.lmax, args.nmax, box_size / 2, false, false, false, false);
      }
      timer.stop();
      std::cout << "\t\t took " << timer.duration_ms()/args.nruns << "ms" << std::endl;
      out << timer.duration_ms()/args.nruns << " ";
    }
  }

  return out.str();
};


class Run {
public:
  explicit Run(const json& json_param) {
    size_t arglen = 1;
    std::vector<std::string> var_param{"n", "lmax", "nmax", "nside", "nruns"};
    for(auto& vp: var_param){
      if(json_param.at(vp).is_array()){
        if(arglen == 1) {
          arglen = json_param.at(vp).size();
        } else if(arglen != json_param.at(vp).size()) {
          throw std::runtime_error("parameter arrays must all have the same length in the same run");
        }
      }
    }
    
    char cm;
    if(json_param.at("compute_method")=="rev")
      cm = ComputeMethod::REV;
    else if(json_param.at("compute_method")=="revraw")
      cm = ComputeMethod::REV | ComputeMethod::RAW;
    else if(json_param.at("compute_method")=="revrevraw")
      cm = ComputeMethod::REV | ComputeMethod::RAW | ComputeMethod::REV_NOFFT;
    else
      throw std::runtime_error(std::string("unknown compute_method") + json_param.at("compute_method").get<std::string>());

    char pm;
    if(json_param.at("interp_method")=="on")
      pm = Interp::YES;
    else if(json_param.at("interp_method")=="off")
      pm = Interp::NO;
    else if(json_param.at("interp_method")=="both")
      pm = Interp::YES | Interp::NO;
    else
      throw std::runtime_error("unknown interp_method" + json_param.at("interp_method").get<std::string>());

    for(size_t i=0; i<arglen; ++i) {
      StepArgs arg;
      arg.n = json_param.at("n").is_array() ? json_param.at("n")[i] : json_param.at("n");
      arg.lmax = json_param.at("lmax").is_array() ? json_param.at("lmax")[i] : json_param.at("lmax");
      arg.nmax = json_param.at("nmax").is_array() ? json_param.at("nmax")[i] : json_param.at("nmax");
      arg.nside = json_param.at("nside").is_array() ? json_param.at("nside")[i] : json_param.at("nside");
      arg.nruns = json_param.at("nruns").is_array() ? json_param.at("nruns")[i] : json_param.at("nruns");
      arg.compute_method = cm;
      arg.interp_method = pm;
      add_step(arg);
    }
  }

  void add_step(StepArgs args) {
    step_container.push_back(args);
  }

  std::string execute() {
    std::stringstream output;
    std::for_each(step_container.begin(), step_container.end(), [&](const auto& args){
      output << execute_step(args) << "\n";
    });
    return output.str();
  };

private:
  std::vector<StepArgs> step_container;
};



int main(int argc, char *argv[]) {
  std::string json_filename(argv[1]);
  std::ifstream json_file(json_filename);
  json params;
  json_file >> params;

  std::vector<Run> runs;
  if(params.is_array()) {
    for(auto& p: params)
      runs.emplace_back(Run(p));
  } else {
    runs.emplace_back(Run(params));
  }

  for(size_t i=0; i<runs.size(); ++i){
    auto result = runs[i].execute();
    std::ofstream f("results_run_" + std::to_string(i) + ".txt");
    f << result;
    f.close();
  }
};