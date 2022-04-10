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
        //std::cout << "---" << '\n';

        // According to the C++ reference docs, "the insertion operation checks whether each inserted element has a key equivalent to the one of an element already in the container, and if so, the element is not inserted"
        // insert hashAsText-File as key-value pair into the original files.
//        const std::string& hashAsText = file->getHash();
//        const File& fileReference = *(file.get());

        // Instead of vector being a container of unique_ptrs for Files and map a container of reference to ref_wrap string-ref wrap File pair
        //   another solution for storing Files would be to make the vector a container of shared_ptrs on Files and the map an container of
        //   weak_ptrs to the string (hashAsText in File) and of weak_ptrs to the file itself
        //   C++ combos:
        //   - 'shared_ptr' and 'weak_ptr'
        //   - 'unique_ptr' and 'reference_wrapper'
//        originalFiles.emplace(hashAsText, fileReference);     // referencing local variables produces unreadable characters and undefined behavior
        // when iterating and printing out contents of the map

        // if the file is missing in the original files
        //   - by checking if the original files container contains the hashAsText key associated with the file -
        //   add it to the original files.
        //   Otherwise add the file to the duplicate files

        bool isFileMissing = originalFiles.count(file->getHash()) == 0;
        if (isFileMissing) {
            originalFiles.emplace(file->getHash(), *(file.get()));
            // or explicitly
            //originalFiles.emplace(file->getHash(), std::cref(*(file.get())));
            continue;
        }

        duplicateFiles.emplace(file->getHash(), *(file.get()));
        const_cast<File&>(originalFiles.at(file->getHash()).get()).addDuplicateFile(*(file.get()));
    }
}

void Directory::moveDuplicatesToSeparateDirectory() {
    std::cout << "\n===================================================================\n\n";
    std::cout << "MOVING DUPLICATE FILES\n\n";

    // TODO multiplatfofrmize
    //  ifdef for Windows - variablize current hard-coded forward slash '/' to 'pathSeparator' that assigns
    //  - forward slash '/' for Linux and Mac, and
    //  - backward slash '\' for Windows
    this->pathToDuplicateFilesDirectoryAsText = this->pathToDirectoryAsText + "/DUPLICATE_FILES/";
    bool endsPathToDirectoryWithForwardSlash = this->pathToDirectoryAsText.at(this->pathToDirectoryAsText.size() - 1) == '/';
    if (endsPathToDirectoryWithForwardSlash) {
        this->pathToDirectoryAsText.pop_back();
    }

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
