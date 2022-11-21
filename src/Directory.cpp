#include "Directory.h"

#include "HashGenerator.h"

#include <algorithm>
#include <cassert>
#include <iostream>

Directory::Directory(std::string pathToDirectory) :
        pathToDirectoryAsText(std::move(pathToDirectory))
{}

const std::string& Directory::getPathToDirectory() const {
    return pathToDirectoryAsText;
}

void Directory::scan() {
    std::filesystem::path aPath {this->getPathToDirectory()};
    std::unique_ptr<HashGenerator> hashGenerator{};

    for (const auto& entry : std::filesystem::directory_iterator(aPath)) {
        const auto absolutePathForFile = entry.path().string();
        if (entry.is_regular_file() ) {
            std::string shaAsString {hashGenerator->sha256_CPP_style(absolutePathForFile)};
            auto hashAsText = std::make_unique<Hash>(std::move(shaAsString));
            auto fileInDirectory = std::make_unique<File>(entry, std::move(hashAsText));

            this->allFilesInDirectory.emplace_back(std::move(fileInDirectory));
        }
    }
}

void Directory::sort() {
    struct FilePathsComparator {
        bool operator()(const std::unique_ptr<File>& firstFile, const std::unique_ptr<File>& secondFile) const {
            return (firstFile.get()->getModifiedAbsolutePath() < secondFile.get()->getModifiedAbsolutePath());
        }
    };

    std::sort(this->allFilesInDirectory.begin(), this->allFilesInDirectory.end(), FilePathsComparator());
}

void Directory::findDuplicates() {
    for (const auto& file : this->allFilesInDirectory) {
        // For debugging purposes
//        std::cout << file->getHash();
//        std::cout << "\t";
//        std::cout << file->getAbsolutePath();
//        std::cout << "\t";
//        std::cout << file->getModifiedAbsolutePath();
//        std::cout << "\n";
//        std::cout << "---" << '\n';

        bool isFileMissing = originalFiles.count(file->getHash()) == 0;
        if (isFileMissing) {
            originalFiles.emplace(file->getHash(), *(file.get()));
            // or explicitly
            //originalFiles.emplace(file->getHash(), std::cref(*(file.get())));
            continue;
        }

        duplicateFiles.emplace(file->getHash(), *(file.get()));
        File& originalFile = const_cast<File&>(originalFiles.at(file->getHash()).get());
        originalFile.addDuplicateFile(*(file.get()));
    }
}

void Directory::moveDuplicatesToSeparateDirectory() {
    std::cout << "\n===================================================================\n\n";
    std::cout << "MOVING DUPLICATE FILES\n\n";

    std::string pathDelimiter{"/"};

#if defined(_WIN32) || defined(__WIN32__)
    pathDelimiter = "\\";
#endif

    bool endsPathToDirectoryWithForwardSlash = this->pathToDirectoryAsText.at(this->pathToDirectoryAsText.size() - 1) == '/';
    if (endsPathToDirectoryWithForwardSlash) {
        this->pathToDirectoryAsText.pop_back();
    }

    this->pathToDuplicateFilesDirectoryAsText = this->pathToDirectoryAsText + pathDelimiter + "DUPLICATE_FILES" + pathDelimiter;

    std::filesystem::create_directories(this->pathToDuplicateFilesDirectoryAsText);

    for (const auto& [hash, duplicateFile] : this->duplicateFiles) {
        try {
            std::cout << "Moving duplicate file\n";
            std::cout << "\t" << duplicateFile.get().getAbsolutePath() << "\n";
            std::cout << "to a separate directory for duplicate files\n";
            std::cout << "\t" << this->pathToDuplicateFilesDirectoryAsText + duplicateFile.get().getFilename() << "\n";
            std::cout << "---\n";

            const std::string& from = duplicateFile.get().getAbsolutePath();
            const std::string& to = this->pathToDuplicateFilesDirectoryAsText + duplicateFile.get().getFilename();
            std::filesystem::rename(from, to);

        } catch (std::filesystem::filesystem_error& e) {
            std::cout << e.what() << '\n';
        }
    }
}

void Directory::verifyFiles() {
    std::cout << "\n===================================================================\n\n";
    std::cout << "Before move:\n";
    std::cout
            << "Found " << this->allFilesInDirectory.size() << " files:\n"
            << this->originalFiles.size() << " original files and "
            << this->duplicateFiles.size() << " duplicate files" << "\n";

    std::filesystem::path pathToDirectory {this->getPathToDirectory()};

    uint32_t numberOfOriginalFiles{};
    for (const auto& entry : std::filesystem::directory_iterator(pathToDirectory)) {
        if (entry.is_regular_file()) {
            numberOfOriginalFiles++;
        }
    }

    uint32_t numberOfDuplicateFiles{};
    for (const auto& entry : std::filesystem::directory_iterator(this->pathToDuplicateFilesDirectoryAsText)) {
        if (entry.is_regular_file()) {
            numberOfDuplicateFiles++;
        }
    }

    std::cout << "\n";

    std::cout << "After move:\n";
    std::cout
            << "Found:\n" << numberOfOriginalFiles << " original files and "
            << numberOfDuplicateFiles << " duplicate files" << "\n";

    assert(numberOfOriginalFiles == this->originalFiles.size());
    assert(numberOfDuplicateFiles >= this->duplicateFiles.size());

    std::cout << "\n";

    std::cout << "Number of original files before and after duplicate files moving matches.\n";
    std::cout << "Duplicate files moved successfully.\n";
}

const OriginalFilesType& Directory::getOriginalFiles() const {
    return this->originalFiles;
}

const DuplicateFilesType& Directory::getDuplicateFiles() const {
    return this->duplicateFiles;
}
