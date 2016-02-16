//
// Created by Michael Bühlmann on 17/02/16.
//

#include <catana/iotools.hpp>
#include <catana/decomposition/sfb_decomposition.hpp>
#include <iostream>
#include <random>
#include <cmath>
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

int main() {
    int nside = 64;
    int lmax = 10;
    int nmax = 50;
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

        std::cout << oc.size() << " ";

        // Raw
//        time.start();
//        for(size_t i=0; i<r.second; ++i){
//            auto kclkk = decomp_SFB(oc, lmax, nmax, box_size/2., window_volume, false, false);
//        }
//        time.stop();
//        std::cout << time.duration<timer::milliseconds>()/r.second << " ";

        // Raw Parallel
        time.start();
        for(size_t i=0; i<r.second; ++i){
            auto kclkk = decomp_SFB(oc, lmax, nmax, box_size/2., window_volume, false, true);
        }
        time.stop();
        std::cout << time.duration<timer::milliseconds>()/r.second << " ";

        // Reverse
//        time.start();
//        for(size_t i=0; i<r.second; ++i){
//            auto kclkk = decomp_SFB(pix_oc, lmax, nmax, box_size/2., window_volume, false, false);
//        }
//        time.stop();
//        std::cout << time.duration<timer::milliseconds>()/r.second << " ";

        // Reverse, Parallel
        time.start();
        for(size_t i=0; i<r.second; ++i){
            auto kclkk = decomp_SFB(pix_oc, lmax, nmax, box_size/2., window_volume, false, true);
        }
        time.stop();
        std::cout << time.duration<timer::milliseconds>()/r.second << std::endl;
    }
    return 0;
}

