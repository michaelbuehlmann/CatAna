//
// Created by Michael Bühlmann on 11/02/16.
//

#ifndef CATANA_OBJECTCONTAINER_HPP
#define CATANA_OBJECTCONTAINER_HPP

#include "Object.hpp"
#include <random>
#include <vector>
#include <cassert>

extern std::mt19937 rng;

//! Catalog representation
using ObjectContainer = std::vector<Object>;

//! Creates a random subset of the Object Container such that N objects remain.
/*
 * This is a Fisher-Yates shuffle (stopped after N iterations).
 * See: https://en.wikipedia.org/wiki/Fisher–Yates_shuffle
 */
void create_object_container_subset(ObjectContainer& object_container, const size_t& n);

#endif //CATANA_OBJECTCONTAINER_HPP
