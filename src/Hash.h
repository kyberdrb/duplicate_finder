#pragma once

#include <ostream>
#include <string>

class Hash {
public:
    explicit Hash(std::string hashAsText);

    friend bool operator<(const Hash& firstHash, const Hash& secondHash) {
        return firstHash.hashAsText < secondHash.hashAsText;
    }

    friend std::ostream& operator<<(std::ostream& out, const Hash& hash) {
        out << hash.hashAsText;
        return out;
    }

private:
    std::string hashAsText;
};
