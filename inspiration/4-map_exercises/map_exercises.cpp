#include "File.h"

#include <iostream>

#include <fstream>
#include "openssl/sha.h"
#include <vector>
#include <array>
#include <sstream>
#include <cstdint>
#include <iomanip>

#include <list>

#include <map>
#include <functional>

std::map<std::string, int> myMap;
myMap.emplace(std::make_pair("earth", 1));
myMap.emplace(std::make_pair("moon", 2));

std::map<int, std::reference_wrapper<const File>> mb1;
const File& f1 = *(allFilesInDirectory.begin()->get());
mb1.emplace(0, std::cref(f1));

std::map<std::string, std::reference_wrapper<const File>> mb2;
//std::map<std::string, std::reference_wrapper<const File>, stringComparator> mb2;
const File& f2 = *(allFilesInDirectory.begin()->get());
mb2.emplace("hello", std::cref(f2));

struct stringComparator {
    bool operator()(const std::string& a, const std::string& b) const {
        return a < b;
    }
};

//std::map<std::reference_wrapper<const std::string>, std::reference_wrapper<const File>> mb3;
std::map<std::reference_wrapper<const std::string>, std::reference_wrapper<const File>, stringComparator> mb3;
const File& f3 = *(allFilesInDirectory.begin()->get());
const std::string key {"hello"};
mb3.emplace(key, std::cref(f3));

auto&& tempStringComparator = [](const std::string& a, const std::string& b) mutable { return a < b; };
    auto& stringComparator = tempStringComparator;
    std::map<std::reference_wrapper<
        const std::string>,                 // reference to 'hash' member from File object from the vector
        std::reference_wrapper<const File>, // reference to the File object from the vector
        decltype(stringComparator)> originalFiles (stringComparator);
















