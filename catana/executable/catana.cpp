//
// Created by Michael Bühlmann on 05/05/16.
//

#include <iostream>
#include <string>
#include <random>
#include <catana/io.hpp>
#include <catana/decomposition.hpp>

// Provide a random number generator
std::random_device rand_dev;
std::mt19937 rng;

void init_random(){
    std::array<int, 624> seed_data;
    std::generate(seed_data.begin(), seed_data.end(), std::ref(rand_dev));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    rng.seed(seq);
}

bool is_power_of_2(const int& n)
{
    return (((n & (n-1))==0));
}

void print_usage(){
    std::cout
            << "CatAna: Catalog Analyzer\n"
            << "A library for discrete 3D spherical Fourier Bessel transformation\n\n";
    std::cout
            << "Usage:  catana_exec catalog_file RMAX LMAX NMAX NSIDE\n\n";
    std::cout
            << "catalog_file: [string] text file, each row [radius, theta, phi] of galaxy\n"
            << "    radius: r>0, in Mpc\n"
            << "    theta:  lattitude, in [0,pi)\n"
            << "    phi:    longitude, in [0,2pi)\n"
            << "RMAX:  [float] radius of boundary condition, in Mpc\n"
            << "LMAX:  [uint]  maximal l-multipole to compute\n"
            << "NMAX:  [uint]  number of k for each l to compute SFB transform\n"
            << "NISDE: [uint, 2^x] HEALPix resolution\n";
}

int main(int argc, char* argv[]){
    if(argc != 6){
        std::cout << "Invalid number of arguments.\n\n";
        print_usage();
        return EXIT_FAILURE;
    }

    std::string filename(argv[1]);
    double rmax = std::atof(argv[2]);
    if(rmax <=0){
        std::cout << "Invalid RMAX\n";
        return EXIT_FAILURE;
    }
    int lmax = std::atoi(argv[3]);
    if(lmax < 0){
        std::cout << "Invalid LMAX\n";
        return EXIT_FAILURE;
    }
    int nmax = std::atoi(argv[4]);
    if(nmax < 0){
        std::cout << "Invalid NMAX\n";
        return EXIT_FAILURE;
    }
    int nside = std::atoi(argv[5]);
    if(nside < 0 || !is_power_of_2(nside)){
        std::cout << "Invalid NSIDE\n";
        return EXIT_FAILURE;
    }

    init_random();

    catana::io::TextSource<catana::io::SphericalRecord<double>> source(filename,1,0);
    catana::io::TophatRadialWindowFunctionFilter tophat(rmax);
    catana::Analyzer analyzer(&source, 1);
    analyzer.add_filter(&tophat);

    auto kclkk = analyzer.compute_sfb_pixelized(lmax, nmax, rmax, nside, false, true);
    kclkk.savetxt("output");

    return EXIT_SUCCESS;
}