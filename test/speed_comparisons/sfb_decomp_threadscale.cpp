//
// Created by Michael Bühlmann on 04/03/16.
//

#include <catana/decomposition.hpp>
#include <catana/types.hpp>

#include "timer.hpp"
#include <random>
#include <string>
#include <iostream>
#include <algorithm>

#include <omp.h>
#include <unistd.h>

using namespace catana;

std::mt19937 rng;

PointContainer random_points(size_t n, double box_size) {
    PointContainer oc(n);
    io::TophatRadialWindowFunctionFilter tophat_filter(box_size/2.);

#pragma omp parallel
    {
        std::mt19937 rng_private;
        std::uniform_real_distribution<double> dist(-box_size/2., box_size/2.);
        Point point;
#pragma omp for
        for (size_t i = 0; i<n; ++i) {
            do{
                point = Point(dist(rng_private), dist(rng_private), dist(rng_private));
            } while(!tophat_filter.filter(point));
            oc[i] = point;
        }
    }
    return oc;
}

int main(int argc, char* argv[]){
//    int lmax = 40;
//    int nmax = 180;
//    int nside = 256;
//    int n = 4000000;

    int lmax=20;
    int nmax = 100;
    int nside=256;
    int n = 4000000;

    int process_id = ::getpid();

    PointContainer oc = random_points(n, 1000.);
    PixelizedPointContainer poc(nside, oc);

    timer::Timer timer;

    for(int i=1; i<argc; ++i){
        int nthreads = std::stoi(argv[i]);
        omp_set_num_threads(nthreads);
        int runs = std::min(nthreads, 8);
        timer.start();
        for(int j=0; j<nthreads; ++j) {
            auto kclkk = _sfb_reverse_fft(poc, lmax, nmax, 500., 523598775, false, false, true, true);
            kclkk.savetxt(std::string("test_")+std::to_string(process_id));
        }
        timer.stop();

        double duration = timer.duration()/nthreads;

        std::cout << nthreads << " " << duration << std::endl;
    }
    return 0;
}