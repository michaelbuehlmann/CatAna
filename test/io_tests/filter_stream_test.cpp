//
// Created by Michael Bühlmann on 11/02/16.
//

#include <catana/types.hpp>
#include <catana/io_tools/filters.hpp>
#include <catana/io_tools/FilterStream.hpp>

#include <gtest/gtest.h>

// Load data directory
#include "config.hpp"
const std::string test_data_dir(TEST_DATA_DIR);

TEST(FilterStream, SmallTophat)
{
    ObjectContainer object_container;
    object_container.push_back(Object(0, 0, 0.8));
    object_container.push_back(Object(0, 0, 1.2));
    object_container.push_back(Object(1, 1, 1));
    object_container.push_back(Object(0.5, 0.5, 0.5));

    ObjectContainer object_container_result;

    ObjectContainerSource source(object_container);
    ObjectContainerSink sink(object_container_result);

    FilterStream filter_stream(&source, &sink, 10);
    TophatRadialWindowFunctionFilter<float> filter(1.);
    filter_stream.add_filter(&filter);
    filter_stream.run();

    EXPECT_EQ(2, object_container_result.size());
    EXPECT_FLOAT_EQ(0.8, object_container_result[0].r);
    EXPECT_FLOAT_EQ(0.8660254038, object_container_result[1].r);
}


TEST(FilterStream, LargeTophat)
{
    ObjectContainer object_container;
    int N = 1<<11;
    for(int i=0; i<N; ++i){
        object_container.push_back(object_from_spherical_position<float>((2.f*i)/N, 0.f, 0.f));
    }

    ObjectContainer object_container_result;

    ObjectContainerSource source(object_container);
    ObjectContainerSink sink(object_container_result);

    FilterStream filter_stream(&source, &sink, 100);
    TophatRadialWindowFunctionFilter<float> filter(1.);
    filter_stream.add_filter(&filter);
    filter_stream.run();

    EXPECT_NEAR(object_container_result.size(), N/2., 1);
}
