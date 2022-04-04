#include "File.h"

File::File(
    std::filesystem::directory_entry fileOnFilesystem,
    std::string hash)
:
//    fileOnFilesystem(fileOnFilesystem),
//    hash(hash)
    fileOnFilesystem(std::move(fileOnFilesystem)),
    hash(std::move(hash))
{}

std::string File::getAbsolutePath() const {
    return this->fileOnFilesystem.path().string();
}

// TODO instead of truncating extension, replace all spaces with underscores
//  - underscore has bigger code than space so the duplicatefile gets under the original file
std::string File::getAbsolutePathWithoutExtension() {
    auto path = this->fileOnFilesystem.path().string();

    while (path.back() != '.') {
        path.pop_back();
    }

    // remove the dot
    path.pop_back();

    return path;
}

std::string File::getHash() const {
    return this->hash;
};
