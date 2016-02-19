//
// Created by Michael Bühlmann on 17/02/16.
//

#include <catana/iotools.hpp>
#include <catana/decomposition/sfb_decomposition.hpp>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <string>
#include "timer.hpp"

std::mt19937 rng;

ObjectContainer random_objects(size_t n, double box_size, bool tophat) {
    std::uniform_real_distribution<double> dist(-box_size/2., box_size/2.);
    ObjectContainer oc;

    for(size_t i=0; i<n; ++i) {
        oc.add_object(Object(dist(rng), dist(rng), dist(rng)));
    }

    if(tophat){
        TophatRadialWindowFunctionFilter tophat_filter(box_size/2.);
        tophat_filter(oc, true);
    }
    return oc;
}

void print_arguments(){
//    std::cout << "Arguments: lmax, nmax, nside, parallel [True/False], methods [raw, hp, hp_fft, hp_fft2, hp_fft3], " << std::endl;
    std::cout << "Arguments: lmax, nmax, nside, parallel [True/False], methods [raw, hp, hp_fft], " << std::endl;
}

int main(int argc, char* argv[]) {
    if(argc < 5){
        std::cout << "Got " << argc << " arguments." << std::endl;
        print_arguments();
        return -1;
    }

    // methods
    bool do_raw=false;
    std::vector<KClkk (*)(const PixelizedObjectContainer&, unsigned short, unsigned short, double, double, bool, bool)> hp_functions;
    // if no method specified: do all
    if(argc==5){
        do_raw=true;
        hp_functions.push_back(decomp_SFB);
        hp_functions.push_back(decomp_SFB_FFT);
//        hp_functions.push_back(decomp_SFB_FFT_v2);
//        hp_functions.push_back(decomp_SFB_FFT_v3);
    } else {
        for(int i=5; i<argc; ++i){
            if(std::string(argv[i]) == std::string("raw")){
                do_raw=true;
            } else if(std::string(argv[i]) == std::string("hp")){
                hp_functions.push_back(decomp_SFB);
            } else if(std::string(argv[i]) == std::string("hp_fft")){
                hp_functions.push_back(decomp_SFB_FFT);
//            } else if(std::string(argv[i]) == std::string("hp_fft2")){
//                hp_functions.push_back(decomp_SFB_FFT_v2);
//            } else if(std::string(argv[i]) == std::string("hp_fft3")){
//                hp_functions.push_back(decomp_SFB_FFT_v3);
            } else {
                std::cout << "Unknown method: " << argv[i] << std::endl;
                print_arguments();
                return -1;
            }
        }
    }

    int nside = std::stoi(argv[3]);
    int lmax = std::stoi(argv[1]);
    int nmax = std::stoi(argv[2]);

    // parallel
    bool parallel;
    if(std::string(argv[4])==std::string("True"))
        parallel=true;
    else if(std::string(argv[4])==std::string("False"))
        parallel=false;
    else{
        std::cout << "Parallel either 'True' or 'False' (got '" << argv[4] << "')" << std::endl;
        return -1;
    }

#if defined(_OPENMP)
#pragma omp parallel if(parallel)
    {
#pragma omp master
        {
                std::cout << "Using " << omp_get_num_threads() << " of max " << omp_get_max_threads() << std::endl;
        }
    }
#endif // defined(_OPENMP)

    std::vector<std::pair<size_t, size_t>> runs = {
            {1<<10, 20},
            {1<<14, 10},
            {1<<16, 4 },
            {1<<18, 2 },
            {1<<20, 1 }};

    double box_size = 100;
    double window_volume = 4/3.*M_PI*std::pow(box_size/2, 3);

    timer::Timer time;


    for(auto& r: runs) {
        ObjectContainer oc(random_objects(r.first, box_size, true));
        PixelizedObjectContainer pix_oc(nside, oc);

        std::cout << std::setw(10) << oc.size() << " ";

        // Raw
        if(do_raw) {
            time.start();
            for (size_t i = 0; i<r.second; ++i) {
                auto kclkk = decomp_SFB(oc, lmax, nmax, box_size/2., window_volume, false, parallel);
            }
            time.stop();
            std::cout << std::setw(10) << time.duration<timer::milliseconds>()/r.second << " ";
        }

        for(auto& fct: hp_functions){
            time.start();
            for(size_t i=0; i<r.second; ++i){
                auto kclkk = fct(pix_oc, lmax, nmax, box_size/2., window_volume, false, parallel);
            }
            time.stop();
            std::cout << std::setw(10) << time.duration<timer::milliseconds>()/r.second << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}

