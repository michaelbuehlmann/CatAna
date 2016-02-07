//
// Created by Michael Bühlmann on 16/11/15.
//


#include <cmath>
#include "catana/Field.hpp"
#include <catana/tools/fftw_field.hpp>
#include "gtest/gtest.h"

TEST(HelperField, Iterator) {
    double realspace_field[] = {0., 1., 2., 3., 4., 5., 6., 7.};
    field_wrapper<double> fw(realspace_field, 2, false);
    auto start = fw.begin();
    auto it = fw.begin();
    for(int i=0; i<8; ++i){
        EXPECT_DOUBLE_EQ(i, start[i]);
        EXPECT_DOUBLE_EQ(i, *(it++));
    }
}

TEST(HelperField, InlineIterator) {
    double realspace_field[] = {0., 1., 0., 0., 2., 3., 0., 0., 4., 5., 0., 0., 6., 7., 0., 0.};
    field_wrapper<double> fw(realspace_field, 2, true);
    auto start = fw.begin();
    auto it = fw.begin();
    for(int i=0; i<8; ++i){
        EXPECT_DOUBLE_EQ(i, start[i]);
        EXPECT_DOUBLE_EQ(i, *(it++));
    }
}

TEST(FieldBasics, Initialization1) {
    Field f1(16, 1., true);
    Field f2(16, 1., false);
}

TEST(FieldBasics, Initialization2) {
    double realspace_field[] = {1., 1., 1., 1., 1., 1., 1., 1.};
    Field f3(2, 1., realspace_field, true);
    Field f4(2, 1., realspace_field, false);
}

TEST(FieldFFT, Normalization){
    double realspace_field[] = {1., 1., 1., 1., 1., 1., 1., 1.};
    Field f(2, 1., realspace_field, false);
    for(auto p: f.real_field){
        EXPECT_DOUBLE_EQ(1., p);
    }

    f.forward_transform();
    f.backward_transform();

    for(auto p: f.real_field){
        EXPECT_DOUBLE_EQ(8., p);
    }
}

TEST(FieldFFT, InlineNormalization){
    double realspace_field[] = {1., 1., 1., 1., 1., 1., 1., 1.};
    Field f(2, 1., realspace_field, true);
    for(auto p: f.real_field){
        EXPECT_DOUBLE_EQ(1., p);
    }

    f.forward_transform();
    f.backward_transform();

    for(auto p: f.real_field){
        EXPECT_DOUBLE_EQ(8., p);
    }
}

TEST(FieldFFT, Gauss){

}