#include "ReportGenerator.h"

#include <iostream>

ReportGenerator::ReportGenerator(const std::reference_wrapper<const Directory> directory) :
    directory(directory)
{}

void ReportGenerator::generateTerminalReport() const {
    std::cout << "\n===================================================================\n\n";
    std::cout << "ORIGINAL FILES - C++17 iteration\n\n";

    std::vector<std::reference_wrapper<const File>> originalFilesInAnotherForm;

    const auto& originalFiles = this->directory.get().getOriginalFiles();
    for (const auto& [hash, file] : originalFiles) {
        std::cout << hash.get() << "\t" << file.get().getAbsolutePath() << "\n";
        originalFilesInAnotherForm.emplace_back(file);
    }

    std::cout << "\n===================================================================\n\n";
    std::cout << "DUPLICATE FILES - C++11 iteration\n\n";

    const auto& duplicateFiles = this->directory.get().getDuplicateFiles();
    for (const auto& keyValuePair : duplicateFiles) {
        std::cout << keyValuePair.first.get() << "\t" << keyValuePair.second.get().getAbsolutePath() << "\n";
    }

    std::cout << "\n===================================================================\n\n";
    std::cout << "ALL ORIGINAL FILES - sorted by name - WITH THEIR DUPLICATES - if any\n\n";

    struct compareFilePathsForRawFiles {
        bool operator()(const File& firstFile, const File& secondFile) const {
            return (firstFile.getModifiedAbsolutePath() < secondFile.getModifiedAbsolutePath());
        }
    };

    std::sort(originalFilesInAnotherForm.begin(), originalFilesInAnotherForm.end(), compareFilePathsForRawFiles());

    for (const auto& file : originalFilesInAnotherForm) {
        std::cout << file;
    }

    std::cout << "\n===================================================================\n\n";
    std::cout << "ONLY ORIGINAL FILES - sorted by name - THAT HAVE DUPLICATES\n\n";

    // assuming that the collection is already sorted
    for (const auto& file : originalFilesInAnotherForm) {
        if (file.get().hasDuplicates()) {
            std::cout << file;
        }
    }
}
