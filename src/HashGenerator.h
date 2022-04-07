#pragma once

#include <string>

class HashGenerator {
public:
    std::string sha256_CPP_style(const std::string& filePath);
    void sha1_C_style_static_alloc(const char* filePath);
    char* sha1_C_style_dynamic_alloc(const char* filePath);
};
