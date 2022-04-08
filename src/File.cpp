#include "File.h"

File::File(const std::filesystem::directory_entry fileInDirectory, std::unique_ptr<Hash> hash) :
        fileInDirectory(std::move(fileInDirectory)),
    hash(std::move(hash))
{}

std::string File::getAbsolutePath() const {
    return this->fileInDirectory.path().string();
}

// for sorting so that the original file gets listed first among duplicate files with similar name
std::string File::getModifiedAbsolutePath() const {
    std::string modifiedAbsolutePath = this->getAbsolutePath();

    size_t position;
    int numberOfCharacters = 1;
    while ((position = modifiedAbsolutePath.find(" ")) != std::string::npos) {
        modifiedAbsolutePath.replace(position, numberOfCharacters, ".");
    }

    while ((position = modifiedAbsolutePath.find("(")) != std::string::npos) {
        modifiedAbsolutePath.erase(position, numberOfCharacters);
    }

    while ((position = modifiedAbsolutePath.find(")")) != std::string::npos) {
        modifiedAbsolutePath.erase(position, numberOfCharacters);
    }

    return modifiedAbsolutePath;
}

const Hash &File::getHash() const {
    return *(this->hash.get());
}

void File::addDuplicateFile(std::reference_wrapper<const File> duplicateFile) {
    this->duplicateFiles.emplace_back(duplicateFile);
};

bool File::hasDuplicates() const {
    return this->duplicateFiles.size() > 0;
}

std::string File::getFilename() const {
    return this->fileInDirectory.path().filename();
}
