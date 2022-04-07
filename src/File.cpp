#include "File.h"

#include <algorithm>

File::File(
    std::filesystem::directory_entry fileOnFilesystem,
    std::string hash)
:
    // Copy values
//    fileOnFilesystem(fileOnFilesystem),
//    hash(hash)

    // Move values
    fileOnFilesystem(std::move(fileOnFilesystem)),
    hash(std::move(hash))
{}

std::string File::getAbsolutePath() const {
    return this->fileOnFilesystem.path().string();
}

// for sorting so that the original file gets listed first among duplicate files with similar name
std::string File::getModifiedAbsolutePath() const {
    std::string modifiedAbsolutePath = this->fileOnFilesystem.path().string();
    size_t position;
    int numberOfCharacters = 1;
    while ((position = modifiedAbsolutePath.find(" ")) != std::string::npos) {
        modifiedAbsolutePath.replace(position, numberOfCharacters, ".");
    }

    while ((position = modifiedAbsolutePath.find("(")) != std::string::npos) {
        //modifiedAbsolutePath.replace(position, 1, "");
        modifiedAbsolutePath.erase(position, numberOfCharacters);
    }

    while ((position = modifiedAbsolutePath.find(")")) != std::string::npos) {
        //modifiedAbsolutePath.replace(position, 1, "");
        modifiedAbsolutePath.erase(position, numberOfCharacters);
    }

    return modifiedAbsolutePath;
}

const std::string& File::getHash() const {
    return this->hash;
}

void File::addDuplicateFile(std::reference_wrapper<const File> duplicateFile) {
    this->duplicateFiles.emplace_back(duplicateFile);
};

bool File::hasDuplicates() const {
    return this->duplicateFiles.size() > 0;
}
