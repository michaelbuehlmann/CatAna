//
// Created by Michael Bühlmann on 15/02/16.
//

#include <gtest/gtest.h>
#include <catana/types.hpp>
#include <catana/iotools.hpp>

TEST(ObjectContainerSink, Pixelized) {
    ObjectContainer oc;
    for(size_t i=1; i<(1<<12); ++i){
        oc.push_back(object_from_spherical_position(1., 3./i, 6./i));
    }

    PixelizedObjectContainer pix_oc(256);

    PixelizedObjectContainerSink pix_sink(pix_oc);
    long int n = pix_sink.write(oc.begin(), oc.size());
    EXPECT_EQ(oc.size(), n);
    EXPECT_EQ(oc.size(), pix_oc.get_nobjects());
}