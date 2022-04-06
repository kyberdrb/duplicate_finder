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
std::string File::getModifiedAbsolutePath() const {
    std::string modifiedAbsolutePath = this->fileOnFilesystem.path().string();
    size_t position;
    while ((position = modifiedAbsolutePath.find(" ")) != std::string::npos) {
        modifiedAbsolutePath.replace(position, 1, ".");
    }

    while ((position = modifiedAbsolutePath.find("(")) != std::string::npos) {
        modifiedAbsolutePath.replace(position, 1, "");
    }

    while ((position = modifiedAbsolutePath.find(")")) != std::string::npos) {
        modifiedAbsolutePath.replace(position, 1, "");
    }

    return modifiedAbsolutePath;
}

const std::string& File::getHash() const {
    return this->hash;
};
