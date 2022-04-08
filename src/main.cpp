#include "Directory.h"
#include "ReportGenerator.h"

#include <iostream>

int main(int argc, char** argv) {
    std::string pathToDirectory = argv[1];

    if (pathToDirectory.empty()) {
        std::cout << "Usage: duplicate_find <PATH_TO_DIRECTORY>";
        exit(1);
    }

    std::cout << "Scanning directory \"" << pathToDirectory << "\" for duplicate files...\n\n";

    auto directory = std::make_unique<Directory>(std::move(pathToDirectory));

    directory->scan();
    directory->sort();
    directory->findDuplicates();

    auto reportGenerator = std::make_unique<ReportGenerator>(*(directory.get()));
    reportGenerator->generateTerminalReport();
    //reportGenerator->generateFileReport();

    directory->moveDuplicatesToSeparateDirectory();
    directory->verifyFiles();

    return 0;
}
