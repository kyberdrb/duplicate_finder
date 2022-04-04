#pragma once

#include <string>

#include <filesystem>
namespace fs = std::filesystem;

//#include <experimental/filesystem>
//namespace fs = std::experimental::filesystem;

class File {
public:
    File(std::filesystem::directory_entry absolutePath, std::string hash);

    std::string getAbsolutePath() const;

    // assuming all files have the same extension separated by a dot character '.'
    std::string getAbsolutePathWithoutExtension();

    std::string getHash() const;

private:
    std::filesystem::directory_entry fileOnFilesystem;
    std::string hash;
};
