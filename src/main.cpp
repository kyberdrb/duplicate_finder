#include "Directory.h"

#include <iostream>

#include <map>
#include <functional>

#include "HashGenerator.h"

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

    // TODO delegate report generation on separate class 'ReportGenerator'
    // auto reportGenerator = std::make_unique<ReportGenerator>(directory);
    // reportGenerator->generateTerminalReport();
    // reportGenerator->generateFileReport();
    directory->generateReport();

    directory->moveDuplicatesToSeparateDirectory();
    directory->verifyFiles();

    return 0;
}
