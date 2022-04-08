#pragma once

#include "Hash.h"

#include <string>

// Using a Functor for string (or object-type key) comparison
struct HashComparatorAscending {
    bool operator()(const Hash& firstHash, const Hash& secondHash) const {
        return firstHash < secondHash;
    }
};