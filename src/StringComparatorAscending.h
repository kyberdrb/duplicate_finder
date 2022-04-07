#pragma once

#include <string>

// Using a Functor for string (or object-type key) comparison
struct StringComparatorAscending {
    bool operator()(const std::string& a, const std::string& b) const {
        return a < b;
    }
};